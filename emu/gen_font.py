from PIL import Image

image = Image.open("font.png")

pixels = image.load()

print("#include \"common.h\"\n")
print("uint8_t font[] = {")

for cy in range(0, 8):
    for cx in range(0, 16):
        ch = (cy * 16) + cx

        print("\t// " + str(ch))

        for py in range(0, 8):
            binary = ""

            for px in range(0, 8):
                pixel = pixels[(cx * 8) + px, (cy * 8) + py]

                if pixel == (255, 255, 255):
                    binary += "0"
                else:
                    binary += "1"

            print(f"\t{int(binary, 2)},")

print("};")
