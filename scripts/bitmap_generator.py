from PIL import Image, ImageDraw, ImageFont, ImageText

texts = " ~`!@#$%^&*()_-+,.?/\\|:;'\"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

size = 512
font_size = 64

img = Image.new("RGB", (512, 512), "white")
draw = ImageDraw.Draw(img)
font = ImageFont.truetype("RobotoMono-VariableFont_wght.ttf", size=font_size)



max_width = 0
max_height = 0
for t in texts:
    text = ImageText.Text(t, font)
    left, top, right, bottom = text.get_bbox((0,0), "la", "left")
    max_width = max(max_width, right - left)
    max_height = max(max_height, bottom - top)
print(max_width, max_height)

characters_per_line = int(512 // max_width)
y_offset = 0
for i in range(0, len(texts), characters_per_line):
    t = texts[i:i+characters_per_line]
    text = ImageText.Text(t, font)
    left, top, right, bottom = text.get_bbox((0,0), "la", "left")
    draw.text((0, y_offset + top), text, fill="black")

    height = bottom - top
    y_offset += height
    


img.save("bitmap_font.png")
