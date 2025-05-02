from flask import Flask, render_template, request, redirect, url_for
import subprocess

app = Flask(__name__)

def wall_classes(value):
    classes = []
    if value & 8: classes.append('top')
    if value & 1: classes.append('right')
    if value & 2: classes.append('bottom')
    if value & 4: classes.append('left')
    return ' '.join(classes)

def run_maze_generator(size):
    try:
        result = subprocess.run(["maze_gen.exe", str(size)], capture_output=True, text=True, check=True)
        lines = result.stdout.strip().splitlines()
        return [[int(char, 16) for char in line.strip()] for line in lines if line.strip()]
    except subprocess.CalledProcessError as e:
        print("Error running maze_gen.exe:", e)
        return []

@app.route('/', methods=['GET', 'POST'])
def index():
    maze = []
    if request.method == 'POST':
        size = request.form.get("size", type=int)
        if size and size > 1:
            maze = run_maze_generator(size)
            maze = [[wall_classes(cell) for cell in row] for row in maze]
    return render_template("index.html", maze=maze)


if __name__ == "__main__":
    app.run(debug=True)
