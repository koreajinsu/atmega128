집 부분 최종 코드

import time
import threading
import board
import adafruit_dht
import smbus2 as smbus
from RPLCD.i2c import CharLCD
import busio
from adafruit_pca9685 import PCA9685
import RPi.GPIO as GPIO
import speech_recognition as sr

# DHT22 센서 설정
dhtDevice = adafruit_dht.DHT22(board.D4)

# I2C 설정
I2C_BUS = 1
I2C_ADDRESS = 0x26
LCD_COLUMNS = 16
LCD_ROWS = 2

# smbus 객체 생성
bus = smbus.SMBus(I2C_BUS)

# led_status 및 curtain_status 초기화
led_status = "Off"  # 여기에 led_status 초기화 추가
curtain_status = "Idle"
b_motor_auto_control = False  # 에어컨 자동 제어 상태



# LCD 접근을 위한 Lock 생성
lcd_lock = threading.Lock()

def initialize_lcd():
    global lcd
    for attempt in range(5):  # 최대 5회 시도
        try:
            lcd = CharLCD(i2c_expander='PCF8574', address=I2C_ADDRESS, port=I2C_BUS,
                          cols=LCD_COLUMNS, rows=LCD_ROWS, dotsize=8,
                          charmap='A00', auto_linebreaks=True, backlight_enabled=True)
            lcd.clear()
            with lcd_lock:
                lcd.cursor_pos = (0, 0)
                lcd.write_string("Blind".ljust(6))
                lcd.cursor_pos = (0, 6)
                lcd.write_string("Led".ljust(4))
            return  # 성공하면 함수 종료
        except Exception as e:
            print(f"LCD 초기화 오류 (시도 {attempt + 1}/5): {e}")
            time.sleep(1)  # 잠시 대기 후 재시도
    print("LCD 초기화 실패")

# 초기화 함수 호출
initialize_lcd()

def read_dht22():
    while True:
        try:
            temperature = dhtDevice.temperature
            humidity = dhtDevice.humidity
            if temperature is not None and humidity is not None:
                update_lcd_dynamic()
            time.sleep(2)  # DHT22 센서 읽기 주기
        except RuntimeError as e:
            print(f"DHT22 오류: {e}")
        except Exception as e:
            print(f"DHT22 읽기 오류: {e}")
            
# PCA9685 설정
i2c = busio.I2C(board.SCL, board.SDA)
pca = PCA9685(i2c)
pca.frequency = 60

MOTOR_A = {'ena': 8, 'in1': 9, 'in2': 10}
MOTOR_B = {'ena': 13, 'in1': 11, 'in2': 12}
LED_PINS = {'red': 23, 'green': 24, 'blue': 25}
RELAY_PIN = 17
SWITCH_PIN = 26  # 예시: 스위치 핀 번호

GPIO.setmode(GPIO.BCM)
GPIO.setup(list(LED_PINS.values()) + [RELAY_PIN, SWITCH_PIN], GPIO.OUT)
GPIO.setup(SWITCH_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)  # 풀업 저항 설정

def run_motor_for_seconds(motor, direction, speed_percent, duration):
    def run_motor():
        try:
            set_motor(motor, speed_percent, direction)
            time.sleep(duration)
        finally:
            set_motor(motor, 0, 'stop')

    motor_thread = threading.Thread(target=run_motor)
    motor_thread.start()

def cleanup():
    GPIO.cleanup()
    pca.deinit()
    print("프로그램이 종료되었습니다.")

def set_led_color(color):
    # 모든 LED를 꺼줍니다.
    for pin in LED_PINS.values():
        GPIO.output(pin, GPIO.HIGH)  # GPIO.HIGH는 LED를 끄는 것
    
    # 선택된 색의 LED를 켭니다.
    if color:
        GPIO.output(LED_PINS[color], GPIO.LOW)  # GPIO.LOW는 LED를 켜는 것
    


def control_relay(state):
    global led_status
    try:
        GPIO.output(RELAY_PIN, GPIO.HIGH if state == 'on' else GPIO.LOW)
        led_status = 'On' if state == 'on' else 'Off'
        update_lcd_dynamic()
        if state == 'on':
            speak("불을 켭니다.")
        elif state == 'off':
            speak("불을 끕니다.")
    except Exception as e:
        print(f"릴레이 제어 오류: {e}")

def control_curtain(direction):
    global curtain_status
    try:
        if direction == 'up':
            run_motor_for_seconds(MOTOR_A, 'forward', 25, 1)
            curtain_status = 'Up'
            speak("커튼을 올립니다.")
        elif direction == 'down':
            run_motor_for_seconds(MOTOR_A, 'backward', 25, 1)
            curtain_status = 'Down'
            speak("커튼을 내립니다.")
        update_lcd_dynamic()
    except Exception as e:
        print(f"커튼 제어 오류: {e}")

def set_motor(motor, speed_percent, direction=None):
    try:
        speed = min(max(speed_percent, 0), 100)
        duty_cycle = int(speed / 100.0 * 0xFFFF)
        pca.channels[motor['ena']].duty_cycle = duty_cycle

        if direction == 'forward':
            pca.channels[motor['in1']].duty_cycle = 0xFFFF
            pca.channels[motor['in2']].duty_cycle = 0x0000
        elif direction == 'backward':
            pca.channels[motor['in1']].duty_cycle = 0x0000
            pca.channels[motor['in2']].duty_cycle = 0xFFFF
        elif direction == 'stop':
            pca.channels[motor['in1']].duty_cycle = 0x0000
            pca.channels[motor['in2']].duty_cycle = 0x0000
    except Exception as e:
        print(f"모터 제어 오류: {e}")

def update_lcd_static():
    if lcd:
        with lcd_lock:
            try:
                lcd.cursor_pos = (0, 0)
                lcd.write_string("Blind".ljust(6))
                lcd.cursor_pos = (0, 6)
                lcd.write_string("Led".ljust(4))
                lcd.cursor_pos = (1, 0)
                lcd.write_string(curtain_status.ljust(6))
                lcd.cursor_pos = (1, 6)
                lcd.write_string(led_status.ljust(4))
            except Exception as e:
                print(f"LCD 업데이트 오류: {e}")
    else:
        print("LCD 객체가 초기화되지 않았습니다.")

def update_lcd_dynamic():
    if lcd:
        with lcd_lock:
            try:
                # 온도와 습도 값만 업데이트
                temperature = dhtDevice.temperature
                humidity = dhtDevice.humidity

                if temperature is not None and humidity is not None:
                    lcd.cursor_pos = (0, 10)
                    lcd.write_string(f'{temperature:.1f}C'.rjust(6))
                    lcd.cursor_pos = (1, 10)
                    lcd.write_string(f'{humidity:.1f}%'.rjust(6))
                else:
                    lcd.cursor_pos = (0, 10)
                    lcd.write_string('Err'.rjust(6))
                    lcd.cursor_pos = (1, 10)
                    lcd.write_string('Err'.rjust(6))
            except RuntimeError:
                lcd.cursor_pos = (0, 10)
                lcd.write_string('Err'.rjust(6))
                lcd.cursor_pos = (1, 10)
                lcd.write_string('Err'.rjust(6))
            except Exception as e:
                print(f"LCD 업데이트 오류: {e}")
    else:
        print("LCD 객체가 초기화되지 않았습니다.")


def process_voice_command(command):
    global b_motor_auto_control
    try:
        if "불 켜" in command:
            control_relay('on')
        elif "불 꺼" in command:
            control_relay('off')
        elif "커튼 올려" in command:
            control_curtain('up')
        elif "커튼 내려" in command:
            control_curtain('down')
        elif "에어컨 켜" in command:
            set_motor(MOTOR_B, 20, 'forward')  # 에어컨 1단계
            set_led_color('blue')
            
            speak("에어컨을 1단계 속도로 켭니다.")
        elif "에어컨 1단계" in command:
            set_motor(MOTOR_B, 20, 'forward')  # 에어컨 1단계
            set_led_color('red')
            speak("에어컨을 1단계 속도로 켭니다.")
        elif "에어컨 2단계" in command:
            set_motor(MOTOR_B, 60, 'forward')  # 에어컨 2단계
            set_led_color('green')
            speak("에어컨을 2단계 속도로 켭니다.")
        elif "에어컨 3단계" in command:
            set_motor(MOTOR_B, 100, 'forward')  # 에어컨 3단계
            set_led_color('blue')
            speak("에어컨을 3단계 속도로 켭니다.")  # 수정: "2단계"를 "3단계"로
        elif "에어컨 꺼" in command:
            b_motor_auto_control = False
            set_motor(MOTOR_B, 0, 'stop')
            set_led_color(None)
            speak("에어컨을 끕니다.")
        else:
            speak("알 수 없는 명령입니다.")
    except Exception as e:
        print(f"명령 처리 오류: {e}")

def recognize_voice_commands():
    recognizer = sr.Recognizer()
    microphone = sr.Microphone()
    
    while True:
        if GPIO.input(SWITCH_PIN) == GPIO.LOW:
            print("스위치가 눌렸습니다. 음성 명령을 기다립니다...")
            with microphone as source:
                recognizer.adjust_for_ambient_noise(source)
                try:
                    while GPIO.input(SWITCH_PIN) == GPIO.LOW:
                        audio = recognizer.listen(source, timeout=5, phrase_time_limit=10)
                        command = recognizer.recognize_google(audio, language='ko-KR')
                        print(f"명령어 인식: {command}")
                        process_voice_command(command)
                except sr.UnknownValueError:
                    print("다시 말씀해주세요.")
                except sr.RequestError as e:
                    print(f"음성 인식 서비스에 문제가 발생했습니다: {e}")
                except Exception as e:
                    print(f"알 수 없는 오류 발생: {e}")
        else:
            print("버튼을 눌러서 명령어를 입력해주세요.")
            time.sleep(1)

def speak(text):
    try:
        from gtts import gTTS
        import pygame
        import tempfile
        import os
        from pydub import AudioSegment

        tts = gTTS(text=text, lang='ko', slow=False)
        with tempfile.NamedTemporaryFile(delete=False, suffix='.mp3') as mp3_file:
            tts.save(mp3_file.name)
            wav_file = mp3_file.name.replace('.mp3', '.wav')

        sound = AudioSegment.from_mp3(mp3_file.name)
        sound.export(wav_file, format="wav")

        pygame.mixer.init()
        pygame.mixer.music.load(wav_file)
        pygame.mixer.music.play()

        while pygame.mixer.music.get_busy():
            time.sleep(0.1)

        os.remove(mp3_file.name)
        os.remove(wav_file)
    except Exception as e:
        print(f"음성 출력 오류: {e}")

def process_key_input(key):
    try:
        if key == '1':
            control_curtain('up')
        elif key == '2':
            control_curtain('down')
        elif key == '3':
            control_relay('on')
        elif key == '4':
            control_relay('off')
        elif key == '5':
            set_motor(MOTOR_B, 25, 'forward')  # 에어컨 1단계 속도
            speak("에어컨 1단계 속도로 작동합니다.")
            set_led_color('red')
        elif key == '6':
            set_motor(MOTOR_B, 50, 'forward')  # 에어컨 2단계 속도
            speak("에어컨 2단계 속도로 작동합니다.")
            set_led_color('green')
        elif key == '7':
            set_motor(MOTOR_B, 75, 'forward')  # 에어컨 3단계 속도
            speak("에어컨 3단계 속도로 작동합니다.")
            set_led_color('blue')
        elif key == '8':
            set_motor(MOTOR_B, 0, 'stop')  # 에어컨 정지
            speak("에어컨을 끕니다.")
    except Exception as e:
        print(f"키 입력 처리 오류: {e}")

def listen_for_key_input():
    while True:
        key = input("Press 1/2/3/4/5/6 for control: ")
        process_key_input(key)
        time.sleep(0.1)

# 스레드 시작
dht_thread = threading.Thread(target=read_dht22)
dht_thread.daemon = True
dht_thread.start()

key_thread = threading.Thread(target=listen_for_key_input)
key_thread.daemon = True
key_thread.start()

voice_thread = threading.Thread(target=recognize_voice_commands)
voice_thread.daemon = True
voice_thread.start()

try:
    while True:
        update_lcd_static()  # 초기 상태 표시
        time.sleep(1)
except KeyboardInterrupt:
    cleanup()