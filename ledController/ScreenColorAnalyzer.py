
from PIL import ImageEnhance
from desktopmagic.screengrab_win32 import getDisplaysAsImages, getRectAsImage

LOW_THRESHOLD = 10
MID_THRESHOLD = 40
HIGH_THRESHOLD = 240

X_STRIP_LED_CT = 144
Y_STRIP_LED_CT = 40

def setup_led_indices():
    return {"topStrip":{
                    "start":0,
                    "stop":X_STRIP_LED_CT - 1
                    },
                "rStrip":{
                    "start":X_STRIP_LED_CT,
                    "stop":X_STRIP_LED_CT + Y_STRIP_LED_CT
                    },
                "botStrip":{
                    "start":X_STRIP_LED_CT + Y_STRIP_LED_CT + 1,
                    "stop":(X_STRIP_LED_CT * 2) + LED_CT_HEIGHT + 1
                    },
                "lStrip":{
                    "start":(X_STRIP_LED_CT * 2) + (LED_CT_HEIGHT + 2),
                    "stop":(X_STRIP_LED_CT * 2) + (LED_CT_HEIGHT * 2) + 2
                    }
                }

# Return avg color of all pixels and ratio of dark pixels for a given image
def img_avg(img):
    dark_pixels = 1
    mid_range_pixels = 1
    r = 1
    g = 1
    b = 1

    # Win version of imgGrab does not contain alpha channel
    if img.mode == 'RGB':
        img.putalpha(0)

    # Create list of pixels
    pixels = list(img.getdata())

    for red, green, blue, alpha in pixels:
        # Don't count pixels that are too dark
        if red < LOW_THRESHOLD and green < LOW_THRESHOLD and blue < LOW_THRESHOLD:
            dark_pixels += 1
        # Or too light
        elif red > HIGH_THRESHOLD and green > HIGH_THRESHOLD and blue > HIGH_THRESHOLD:
            pass
        else:
            if red < MID_THRESHOLD and green < MID_THRESHOLD and blue < MID_THRESHOLD:
                mid_range_pixels += 1
                dark_pixels += 1
            r += red
            g += green
            b += blue

    n = len(pixels)
    r_avg = r / n
    g_avg = g / n
    b_avg = b / n
    rgb = [r_avg, g_avg, b_avg]

    # If computed average below darkness threshold, set to the threshold
    for index, item in enumerate(rgb):
        if item <= LOW_THRESHOLD:
            rgb[index] = LOW_THRESHOLD

    rgb = (rgb[0], rgb[1], rgb[2])

    data = {
        'rgb': rgb,
        'dark_ratio': float(dark_pixels) / float(n) * 100
    }
    return data

# Grabs screenshot of current window, calls img_avg (including on zones if present)
def screen_avg():
    screen_data = {}

    # Win version uses DesktopMagic for multiple displays
    if params.BUILD == 'win':
        try:
            img = getDisplaysAsImages()
        except IndexError:
            display_check()

    resizedImg = img.resize(LED_CT_WIDTH, LED_CT_HEIGHT + 2)  #x strips to the corners, y strips between x strip ends
    
    





def get_monitor_screenshots():
    return getDisplaysAsImages()