from flask import Flask, render_template, request, redirect, url_for
import subprocess
from pathlib import Path

app = Flask(__name__)

# finds file path
def find_file(filename, search_path):
    search_path = Path(search_path)
    for file in search_path.rglob(filename):
        return file
    return None

# determines which walls are up
def wall_classes(value):
    classes = []
    if value & 8: classes.append('top')
    if value & 1: classes.append('right')
    if value & 2: classes.append('bottom')
    if value & 4: classes.append('left')
    return ' '.join(classes)

def run_maze_generator(size):
    exe_path = find_file("maze_gen.exe", ".")
    if not exe_path:
        print("maze_gen.exe not found.")
        return [], []
    try:
        result = subprocess.run([exe_path, str(size)], capture_output=True, text=True, check=True)
        lines = result.stdout.strip().splitlines()
        # The first `size` lines are the maze
        maze_lines = lines[:size]
        solution_lines = lines[size:]
        maze = [[int(char, 16) for char in line.strip()] for line in maze_lines]
        solution = []
        for line in solution_lines:
            if line.startswith("(") and "," in line:
                x, y = map(int, line.strip("()").split(","))
                solution.append((x, y))
        return maze, solution
    except subprocess.CalledProcessError as e:
        print("Error running maze_gen.exe:", e)
        return [], []

@app.route('/', methods=['GET', 'POST'])
def index():
    wall_maze = []
    solution = []
    if request.method == 'POST':
        size = request.form.get("size", type=int)
        if size and size > 1:
            maze, solution = run_maze_generator(size)
            wall_maze = [[wall_classes(cell) for cell in row] for row in maze]
    return render_template("index.html", maze=wall_maze, solution=solution)

if __name__ == "__main__":
    app.run(debug=True)
