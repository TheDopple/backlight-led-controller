
from PIL import ImageEnhance
from desktopmagic.screengrab_win32 import getDisplaysAsImages, getRectAsImage

LOW_THRESHOLD = 10
MID_THRESHOLD = 40
HIGH_THRESHOLD = 240

X_STRIP_LED_CT = 8
Y_STRIP_LED_CT = 4

# Grabs screenshot of current window, calls img_avg (including on zones if present)
def screen_avg(getTopRow=False, getBotRow=False, getLeftCol=False, getRightCol=False):
    img = getDisplaysAsImages()[0]

    #Use resize for color averaging
    resizedImg = img.resize(size=(X_STRIP_LED_CT, Y_STRIP_LED_CT + 2)).load()  
    
    topRowColorData_bytes = None
    botRowColorData_bytes = None
    leftColColorData_bytes = None
    rightColColorData_bytes = None

    if getTopRow:
        print("Getting screen top row data...")
        #Top row data
        topRowColorData_array = [60]
        for pixelIndex in range(X_STRIP_LED_CT):
            topRowColorData_array.append(pixelIndex)
            topRowColorData_array.append(resizedImg[pixelIndex,0][0])
            topRowColorData_array.append(resizedImg[pixelIndex,0][1])
            topRowColorData_array.append(resizedImg[pixelIndex,0][2])
        topRowColorData_array.append(62)
        print(topRowColorData_array)
        topRowColorData_bytes = bytes(topRowColorData_array)

    return {"topRow":topRowColorData_bytes,
            "botRow":botRowColorData_bytes,
            "leftCol":leftColColorData_bytes,
            "rightCol":rightColColorData_bytes}

"""
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
"""