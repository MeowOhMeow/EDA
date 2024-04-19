import matplotlib.pyplot as plt


def draw_layout(chip_width, chip_height, macros):
    fig, ax = plt.subplots()
    ax.set_xlim(0, chip_width)
    ax.set_ylim(0, chip_height)
    ax.set_aspect("equal")

    for macro in macros:
        macro_name, macro_width, macro_height, x1, y1, x2, y2 = macro
        ax.add_patch(
            plt.Rectangle(
                (x1, y1), macro_width, macro_height, edgecolor="black", facecolor="none"
            )
        )
        ax.text(
            x1 + 0.5 * macro_width,
            y1 + 0.5 * macro_height,
            macro_name,
            ha="center",
            va="center",
        )

    plt.show()


filename = "testcases/output.txt"
with open(filename, "r") as f:
    lines = f.readlines()
    chip_width, chip_height = map(int, lines[0].split())
    macros = []
    for line in lines[1:]:
        macro = line.split()
        macros.append(
            (
                macro[0],
                int(macro[1]),
                int(macro[2]),
                int(macro[3]),
                int(macro[4]),
                int(macro[5]),
                int(macro[6]),
            )
        )

draw_layout(chip_width, chip_height, macros)
