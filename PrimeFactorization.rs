fn prime_factorization(mut n: u32) -> Vec<u32> {
    let mut prime_factors: Vec<u32> = Vec::new();

    while n % 2 == 0 {
        prime_factors.push(2);
        n /= 2;
    }

    let mut factor: u32= 3;
    let mut limit: f32 = (n as f32).sqrt();

    while factor <= limit as u32 {
        while n % factor == 0 {
            prime_factors.push(factor);
            n /= factor;
            limit = (n as f32).sqrt();
        }

        factor += 2;
    }

    if n > 1 {
        prime_factors.push(n);
    }

    return prime_factors;
}

fn main() {
    let mut factors = prime_factorization(204);
    factors.iter().for_each(|factor| print!("{} ", factor));

    println!();

    factors = prime_factorization(1234567890);
    factors.iter().for_each(|factor| print!("{} ", factor));
}
