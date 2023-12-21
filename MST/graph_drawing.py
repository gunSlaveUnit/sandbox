import tkinter as tk
from typing import Optional, Callable

from mst import Graph

WINDOW_TITLE = "Graphania"
WINDOW_WIDTH = 800
WINDOW_HEIGHT = 500

DEFAULT_MARGIN = 16

VERTEX_RADIUS = 20
DOUBLE_VERTEX_RADIUS = VERTEX_RADIUS * 2
VERTEX_BORDER_WIDTH = 2

root = tk.Tk()
root.title(WINDOW_TITLE)
root.geometry(f"{WINDOW_WIDTH}x{WINDOW_HEIGHT}")


def on_mst_button_click():
    global lines

    graph = Graph(lines)
    edges = graph.mst()

    for edge in edges:
        v1_index = edge[0]
        v2_index = edge[1]

        start_x, start_y, _ = vertices[v1_index]
        end_x, end_y, _ = vertices[v2_index]
        canvas.create_line(start_x, start_y, end_x, end_y, fill="red", width=3)


mst_button = tk.Button(root, text="MST", command=on_mst_button_click)
mst_button.grid(row=0, column=0, padx=DEFAULT_MARGIN, pady=DEFAULT_MARGIN)

vertices = []

lines = []

v1_line_index: Optional[int] = None
v2_line_index: Optional[int] = None


def make_vertex(event):
    global vertices

    vertex_in_canvas_x_bounds = VERTEX_RADIUS < event.x < canvas.winfo_width() - VERTEX_RADIUS
    if not vertex_in_canvas_x_bounds:
        return

    vertex_in_canvas_y_bounds = VERTEX_RADIUS < event.y < canvas.winfo_height() - VERTEX_RADIUS
    if not vertex_in_canvas_y_bounds:
        return

    for vertex in vertices:
        x, y, _ = vertex

        vertex_intersect_with_another_x_bounds = x - DOUBLE_VERTEX_RADIUS < event.x < x + DOUBLE_VERTEX_RADIUS
        vertex_intersect_with_another_y_bounds = y - DOUBLE_VERTEX_RADIUS < event.y < y + DOUBLE_VERTEX_RADIUS

        if vertex_intersect_with_another_x_bounds and vertex_intersect_with_another_y_bounds:
            return

    vertices.append((event.x, event.y, len(vertices)))

    canvas.create_oval(
        event.x - VERTEX_RADIUS,
        event.y - VERTEX_RADIUS,
        event.x + VERTEX_RADIUS,
        event.y + VERTEX_RADIUS,
        width=VERTEX_BORDER_WIDTH,
    )


weight_entries = []


def on_weight_entry_click(event):
    widget = event.widget
    widget_text = widget.get()

    if widget_text != '':
        lines[widget.index][2] = int(widget_text)


class Entry(tk.Entry):
    def __init__(self, index, *args, **kwargs):
        self.index = index

        super().__init__(*args, **kwargs)


def make_line(event):
    global v1_line_index
    global v2_line_index
    global vertices

    if v1_line_index is None:
        vertex_index = None
        for index, vertex in enumerate(vertices):
            x, y, _ = vertex
            if x - VERTEX_RADIUS < event.x < x + VERTEX_RADIUS and y - VERTEX_RADIUS < event.y < y + VERTEX_RADIUS:
                vertex_index = index
                break

        v1_line_index = vertex_index
        return

    if v2_line_index is None:
        vertex_index = None
        for index, vertex in enumerate(vertices):
            x, y, _ = vertex
            if x - VERTEX_RADIUS < event.x < x + VERTEX_RADIUS and y - VERTEX_RADIUS < event.y < y + VERTEX_RADIUS:
                vertex_index = index
                break

        v2_line_index = vertex_index

        v1_x, v1_y, _ = vertices[v1_line_index]
        v2_x, v2_y, _ = vertices[v2_line_index]

        lines.append([v1_line_index, v2_line_index, 0])

        canvas.create_line(v1_x, v1_y, v2_x, v2_y, fill="black")

        v1_line_index = None
        v2_line_index = None

        v_x = (v1_x + v2_x) / 2
        v_y = (v1_y + v2_y) / 2

        entry = Entry(len(weight_entries), root, width=3)
        entry.place(
            x=v_x + canvas.winfo_x(),
            y=v_y + canvas.winfo_y(),
        )
        entry.insert(0, "0")
        entry.bind("<KeyRelease>", on_weight_entry_click)

        weight_entries.append(entry)


canvas_on_click_handler: Callable = make_vertex


def on_click(event):
    canvas_on_click_handler(event)


canvas = tk.Canvas(
    root,
    width=WINDOW_WIDTH - 2 * VERTEX_RADIUS,
    height=WINDOW_HEIGHT - 2 * VERTEX_RADIUS,
    background="white",
)
canvas.grid(
    row=0,
    column=1,
    rowspan=3,
    padx=VERTEX_RADIUS,
    pady=VERTEX_RADIUS,
    sticky="nsew",
)
canvas.bind("<Button-1>", on_click)


def set_handler(handler: Callable) -> None:
    global canvas_on_click_handler
    canvas_on_click_handler = handler


vertex_mode_button = tk.Button(root, text="+ vertex", command=lambda: set_handler(make_vertex))
vertex_mode_button.grid(row=1, column=0, padx=DEFAULT_MARGIN, pady=DEFAULT_MARGIN)

line_mode_button = tk.Button(root, text="+ line", command=lambda: set_handler(make_line))
line_mode_button.grid(row=2, column=0, padx=DEFAULT_MARGIN, pady=DEFAULT_MARGIN)

root.grid_rowconfigure(0, weight=1)
root.grid_columnconfigure(1, weight=1)

root.mainloop()
