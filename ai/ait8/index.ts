interface Array<T> {
  fill(value: T): Array<T>;
}

const CANVAS_WIDTH = 1200
const CANVAS_HEIGHT = 600

const STRING_POINTS_AMOUNT = 200
const DX = CANVAS_WIDTH / STRING_POINTS_AMOUNT
const Y_OFFSET = CANVAS_HEIGHT / 2
const TAU = 0.05
var x_start = 0
const LEARNING_ACCURACY_BOUND = 0.01
const LEARNING_WEIGHT = 0.01

const WINDOW_ELEMENTS_SIZE = 4
var learning_point_index = STRING_POINTS_AMOUNT / 2 + 1
var weights = Array<number>(WINDOW_ELEMENTS_SIZE).fill(0)
var u = Array<number>(STRING_POINTS_AMOUNT).fill(0)
var u_new = Array<number>(STRING_POINTS_AMOUNT).fill(0)
var u_t = Array<number>(STRING_POINTS_AMOUNT).fill(0)

var canvas = document.getElementById('canvas') as HTMLCanvasElement
var context = canvas.getContext('2d') as CanvasRenderingContext2D
context.strokeStyle = 'black'
context.lineWidth = 2

var a: Array<number> = [0.9, -5.4, 9.125, 17.875, -132.525, 15.275, 42.25]

function f(x) {
  return -a[4] * Math.sin(a[3] * a[2] * x) * Math.sin(x) + a[0]
}

function calculate_function_position() {
  for (let i = 0; i < STRING_POINTS_AMOUNT; i++)
    u_new[i] = f(x_start + i * TAU)

  u_t.fill(0)

  for (let i = WINDOW_ELEMENTS_SIZE; i < STRING_POINTS_AMOUNT; i++)
    for (let j = 0; j < WINDOW_ELEMENTS_SIZE; j++)
      u_t[i] += weights[j] * u[i - 1 - j]

  let dU = u_t[learning_point_index] - u_new[learning_point_index]

  if (dU < -LEARNING_ACCURACY_BOUND)
    teach(1)
  if (dU > LEARNING_ACCURACY_BOUND)
    teach(-1)

  x_start += DX
}

function teach(e: number) {
  let weight_additives = Array<number>(WINDOW_ELEMENTS_SIZE)
  let sign = -1

  for (let i = 0; i < WINDOW_ELEMENTS_SIZE; i++) {
    if (u[learning_point_index - 1 - i] < 0)
      sign = -1
    else
      sign = 1

    weight_additives[i] = LEARNING_WEIGHT * sign * e
  }

  for (let i = 0; i < WINDOW_ELEMENTS_SIZE; i++)
    weights[i] += weight_additives[i]
}

let current_step = 0

function draw_frame() {
  context.clearRect(0, 0, canvas.width, canvas.height)

  context.beginPath()
  context.strokeStyle = 'black'
  draw_axes()
  context.stroke()

  context.beginPath()
  context.strokeStyle = 'red'
  context.moveTo(0, f(0) + Y_OFFSET);
  for (let i = 1; i < STRING_POINTS_AMOUNT; i++)
    context.lineTo(i * DX, u_new[i] + Y_OFFSET);
  context.stroke()

  context.beginPath()
  context.strokeStyle = 'blue'
  context.moveTo(0, f(0) + Y_OFFSET);
  console.log(u_t)
  for (let i = 1; i < STRING_POINTS_AMOUNT; i++)
    context.lineTo(i * DX, u_t[i] + Y_OFFSET);
  context.stroke()

  current_step++

  document.getElementById('step').textContent = "Step: " + current_step
  document.getElementById('dh').textContent = "dH: " + LEARNING_ACCURACY_BOUND
  document.getElementById('dg').textContent = "dG: " + LEARNING_WEIGHT
  document.getElementById('g0').textContent = "G[0]: " + weights[0]
  document.getElementById('g1').textContent = "G[1]: " + weights[1]
  document.getElementById('g2').textContent = "G[2]: " + weights[2]
  document.getElementById('g3').textContent = "G[3]: " + weights[3]
}

function draw_axes() {
  context.moveTo(CANVAS_WIDTH / 2, 0)
  context.lineTo(CANVAS_WIDTH / 2, CANVAS_HEIGHT)

  context.moveTo(0, CANVAS_HEIGHT / 2)
  context.lineTo(CANVAS_WIDTH, CANVAS_HEIGHT / 2)
}

function main() {
  calculate_function_position()

  for (let i = 0; i < STRING_POINTS_AMOUNT; i++) {
    u[i] = u_new[i]
  }

  draw_frame()
  requestAnimationFrame(main)
}

requestAnimationFrame(main)