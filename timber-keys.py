import pyautogui
import time
import pydirectinput

class TimberBot:
    def check_branch(self):
        rgb_left=pyautogui.pixel(780,550)
        rgb_right=pyautogui.pixel(1080,550)
        rgb_srodek=pyautogui.pixel(989,681)
        rgb_left_b=pyautogui.pixel(814,636)
        rgb_right_b=pyautogui.pixel(1106,636)
        if rgb_srodek[1] < 100:
            return "Srodek"
        elif rgb_left_b[1] > 240 and not(abs(rgb_left[0]-74)<4):
            return "Right"
        elif rgb_right_b[1] > 240 and not(abs(rgb_right[0]-33)<4):
            return "Left"
        elif abs(rgb_left[0]-74)<4:
            return "Left"
        elif abs(rgb_right[0]-33)<4:
            return "Right"
        else:
            return "None"
    def run(self):
        last_pressed_left=True
        while True:
            side=self.check_branch()
            if side=="Srodek":
                pydirectinput.keyDown('d')
                pydirectinput.press('f')
                pydirectinput.keyUp("d")
            elif side=="Left":
                pydirectinput.press('f')
                pydirectinput.press('f')
                last_pressed_left=False
            elif side=="Right":
                pydirectinput.press('d')
                pydirectinput.press('d')
                last_pressed_left=True
            else:
                if last_pressed_left:
                     pydirectinput.press('d')
                else:
                    pydirectinput.press('f')
if __name__=='__main__':
    time.sleep(2)
    timberBot=TimberBot()
    timberBot.run()
