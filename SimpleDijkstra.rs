struct Label {
    from: i32,
    distance: i32,
    passed: bool
}

fn main() {
    println!("Enter a number of vertices: ");

    let mut vertices_amount = String::new();
    std::io::stdin()
        .read_line(&mut vertices_amount)
        .expect("Failed to read vertices amount");

    let vertices_amount: usize = vertices_amount.trim().parse().expect("Vertices amount isn't a positive number");

    let mut vertices: Vec<Vec<i32>> = vec![vec![0; vertices_amount]; vertices_amount];

    for i in 0..vertices_amount {
        for j in i..vertices_amount {
            if i == j {
                vertices[i][j] = 0;
                continue;
            }

            println!("Enter a distance between {i} and {j}: ");

            let mut distance = String::new();
            std::io::stdin()
                .read_line(&mut distance)
                .expect("Failed to read distance");

            let distance: i32 = distance.trim().parse().expect("Distance isn't a positive number");

            vertices[i][j] = distance;
            vertices[j][i] = distance;
        }
    }

    let mut from = String::new();
    std::io::stdin()
        .read_line(&mut from)
        .expect("Failed to read a start vertex");

    let from: usize = from.trim().parse().expect("Start vertex isn't a positive number");

    let mut to = String::new();
    std::io::stdin()
        .read_line(&mut to)
        .expect("Failed to read an end vertex");

    let to: usize = to.trim().parse().expect("End vertex isn't a positive number");

    let mut labels: Vec<Label> = Vec::new();
    labels.push(Label { from: -1, distance: 0, passed: false });
    for _ in 1..vertices_amount {
        labels.push(Label {from: -1, distance: i32::MAX, passed: false});
    }

    let mut min_vertex = from;
    loop {
        let mut min_distance = i32::MAX;

        for i in 0..vertices_amount {
            if labels[i].distance < min_distance && !labels[i].passed {
                min_vertex = i;
                min_distance = labels[i].distance;
            }
        }

        if min_vertex == to {
            break;
        }

        for i in 0..vertices_amount {
            if vertices[min_vertex][i] > 0 {
                if labels[i].distance > labels[min_vertex].distance + vertices[min_vertex][i] {
                    labels[i] = Label {from: min_vertex as i32, distance: labels[min_vertex].distance + vertices[min_vertex][i], passed: false}
                }
            }
        }

        labels[min_vertex].passed = true;
    }

    loop {
        print!("{min_vertex} ");

        if min_vertex == from {
            break;
        }

        min_vertex = labels[min_vertex].from as usize;
    }
}
