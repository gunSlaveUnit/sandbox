var CANVAS_WIDTH = 1200;
var CANVAS_HEIGHT = 600;
var STRING_POINTS_AMOUNT = 200;
var DX = CANVAS_WIDTH / STRING_POINTS_AMOUNT;
var Y_OFFSET = CANVAS_HEIGHT / 2;
var TAU = 0.05;
var x_start = 0;
var LEARNING_ACCURACY_BOUND = 0.01;
var LEARNING_WEIGHT = 0.01;
var WINDOW_ELEMENTS_SIZE = 4;
var learning_point_index = STRING_POINTS_AMOUNT / 2 + 1;
var weights = Array(WINDOW_ELEMENTS_SIZE).fill(0);
var u = Array(STRING_POINTS_AMOUNT).fill(0);
var u_new = Array(STRING_POINTS_AMOUNT).fill(0);
var u_t = Array(STRING_POINTS_AMOUNT).fill(0);
var canvas = document.getElementById('canvas');
var context = canvas.getContext('2d');
context.strokeStyle = 'black';
context.lineWidth = 2;
var a = [0.9, -5.4, 9.125, 17.875, -132.525, 15.275, 42.25];
function f(x) {
    return -a[4] * Math.sin(a[3] * a[2] * x) * Math.sin(x) + a[0];
}
function calculate_function_position() {
    for (var i = 0; i < STRING_POINTS_AMOUNT; i++)
        u_new[i] = f(x_start + i * TAU);
    u_t.fill(0);
    for (var i = WINDOW_ELEMENTS_SIZE; i < STRING_POINTS_AMOUNT; i++)
        for (var j = 0; j < WINDOW_ELEMENTS_SIZE; j++)
            u_t[i] += weights[j] * u[i - 1 - j];
    var dU = u_t[learning_point_index] - u_new[learning_point_index];
    if (dU < -LEARNING_ACCURACY_BOUND)
        teach(1);
    if (dU > LEARNING_ACCURACY_BOUND)
        teach(-1);
    x_start += DX;
}
function teach(e) {
    var weight_additives = Array(WINDOW_ELEMENTS_SIZE);
    var sign = -1;
    for (var i = 0; i < WINDOW_ELEMENTS_SIZE; i++) {
        if (u[learning_point_index - 1 - i] < 0)
            sign = -1;
        else
            sign = 1;
        weight_additives[i] = LEARNING_WEIGHT * sign * e;
    }
    for (var i = 0; i < WINDOW_ELEMENTS_SIZE; i++)
        weights[i] += weight_additives[i];
}
var current_step = 0;
function draw_frame() {
    context.clearRect(0, 0, canvas.width, canvas.height);
    context.beginPath();
    context.strokeStyle = 'black';
    draw_axes();
    context.stroke();
    context.beginPath();
    context.strokeStyle = 'red';
    context.moveTo(0, f(0) + Y_OFFSET);
    for (var i = 1; i < STRING_POINTS_AMOUNT; i++)
        context.lineTo(i * DX, u_new[i] + Y_OFFSET);
    context.stroke();
    context.beginPath();
    context.strokeStyle = 'blue';
    context.moveTo(0, f(0) + Y_OFFSET);
    console.log(u_t);
    for (var i = 1; i < STRING_POINTS_AMOUNT; i++)
        context.lineTo(i * DX, u_t[i] + Y_OFFSET);
    context.stroke();
    current_step++;
    document.getElementById('step').textContent = "Step: " + current_step;
    document.getElementById('dh').textContent = "dH: " + LEARNING_ACCURACY_BOUND;
    document.getElementById('dg').textContent = "dG: " + LEARNING_WEIGHT;
    document.getElementById('g0').textContent = "G[0]: " + weights[0];
    document.getElementById('g1').textContent = "G[1]: " + weights[1];
    document.getElementById('g2').textContent = "G[2]: " + weights[2];
    document.getElementById('g3').textContent = "G[3]: " + weights[3];
}
function draw_axes() {
    context.moveTo(CANVAS_WIDTH / 2, 0);
    context.lineTo(CANVAS_WIDTH / 2, CANVAS_HEIGHT);
    context.moveTo(0, CANVAS_HEIGHT / 2);
    context.lineTo(CANVAS_WIDTH, CANVAS_HEIGHT / 2);
}
function main() {
    calculate_function_position();
    for (var i = 0; i < STRING_POINTS_AMOUNT; i++) {
        u[i] = u_new[i];
    }
    draw_frame();
    requestAnimationFrame(main);
}
requestAnimationFrame(main);
