var STRING_POINTS_AMOUNT = 500;
var TAU = 0.1;
var GRID_STEP = 1;
var WAVE_SPEED = 2;
var LEARNING_ACCURACY_BOUND = 0.01;
var LEARNING_WEIGHT = 0.01;
var AMPLITUDE_MULTIPLICITY = 3;
var CANVAS_WIDTH = 1200;
var CANVAS_HEIGHT = 600;
var dx = (CANVAS_WIDTH - 1) / (STRING_POINTS_AMOUNT - 1);
var r = Math.pow(WAVE_SPEED * TAU / GRID_STEP, 2);
var learning_point_index = STRING_POINTS_AMOUNT / 2 + 1;
var current_step = 1;
var errors_amount = 0;
var weights = Array(STRING_POINTS_AMOUNT).fill(0);
var ut = Array(STRING_POINTS_AMOUNT).fill(0);
var u = Array(STRING_POINTS_AMOUNT).fill(0);
var un = Array(STRING_POINTS_AMOUNT).fill(0);
var us = Array(STRING_POINTS_AMOUNT).fill(0);
var f = Array(STRING_POINTS_AMOUNT).fill(0);
us[5] = u[5] - TAU * 100;
var canvas = document.getElementById('canvas');
var context = canvas.getContext('2d');
context.strokeStyle = 'black';
context.lineWidth = 2;
function new_step() {
    for (var i = 1; i < STRING_POINTS_AMOUNT - 1; i++)
        f[i] = u[i + 1] - 2 * u[i] + u[i - 1];
    for (var i = 1; i < STRING_POINTS_AMOUNT - 1; i++)
        un[i] = 2 * u[i] - us[i] + r * f[i];
    ut.fill(0);
    for (var j = 1; j < STRING_POINTS_AMOUNT - 1; j++) {
        for (var i = -1; i <= 1; i++)
            ut[j] += weights[1 + i] * u[j + i];
        ut[j] += weights[3] * us[j];
    }
    var dU = ut[learning_point_index] - un[learning_point_index];
    if (dU < -LEARNING_ACCURACY_BOUND)
        teach(1);
    if (dU > LEARNING_ACCURACY_BOUND)
        teach(-1);
    current_step++;
}
function teach(e) {
    var weight_additives = Array(4);
    var sign = -1;
    for (var i = -1; i <= 1; i++) {
        if (u[learning_point_index + i] < 0)
            sign = -1;
        else
            sign = 1;
        weight_additives[1 + i] = LEARNING_WEIGHT * sign * e;
    }
    if (us[learning_point_index] < 0)
        sign = -1;
    else
        sign = 1;
    weight_additives[3] = LEARNING_WEIGHT * sign * e;
    for (var i = 0; i < 4; i++)
        weights[i] += weight_additives[i];
    errors_amount++;
}
function draw_frame() {
    context.clearRect(0, 0, canvas.width, canvas.height);
    context.beginPath();
    context.strokeStyle = 'black';
    draw_axes();
    context.stroke();
    context.beginPath();
    context.strokeStyle = 'blue';
    var y_c = CANVAS_HEIGHT / 2;
    context.moveTo(0, y_c - AMPLITUDE_MULTIPLICITY * u[0]);
    for (var i = 1; i < STRING_POINTS_AMOUNT; i++) {
        var x = i * dx;
        var y = (y_c - AMPLITUDE_MULTIPLICITY * u[i]);
        context.lineTo(x, y);
    }
    context.stroke();
    context.beginPath();
    context.strokeStyle = 'red';
    context.moveTo(0, y_c - AMPLITUDE_MULTIPLICITY * ut[0]);
    for (var i = 1; i < STRING_POINTS_AMOUNT; i++) {
        var x = i * dx;
        var y = (y_c - AMPLITUDE_MULTIPLICITY * ut[i]);
        context.lineTo(x, y);
    }
    context.lineTo(CANVAS_WIDTH, CANVAS_HEIGHT / 2);
    context.stroke();
    document.getElementById('step').textContent = "Step: " + current_step;
    document.getElementById('dh').textContent = "dH: " + LEARNING_ACCURACY_BOUND;
    document.getElementById('dg').textContent = "dG: " + LEARNING_WEIGHT;
    document.getElementById('g0').textContent = "0.04 G[0]: " + weights[0];
    document.getElementById('g1').textContent = "1.92 G[1]: " + weights[1];
    document.getElementById('g2').textContent = "0.04 G[2]: " + weights[2];
    document.getElementById('g3').textContent = "-1   G[3]: " + weights[3];
}
function draw_axes() {
    context.moveTo(CANVAS_WIDTH / 2, 0);
    context.lineTo(CANVAS_WIDTH / 2, CANVAS_HEIGHT);
    context.moveTo(0, CANVAS_HEIGHT / 2);
    context.lineTo(CANVAS_WIDTH, CANVAS_HEIGHT / 2);
}
function main() {
    new_step();
    for (var i = 0; i < STRING_POINTS_AMOUNT; i++) {
        us[i] = u[i];
        u[i] = un[i];
    }
    draw_frame();
    requestAnimationFrame(main);
}
requestAnimationFrame(main);
