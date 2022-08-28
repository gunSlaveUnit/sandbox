fn gcd(mut a: u32, mut b: u32) -> u32 {
    /// GCD - Greatest Common Divisor.
    /// Calculated by the Euclidean algorithm.
    while (b != 0) {
        let remainder = a % b;
        a = b;
        b = remainder;
    }

    return a;
}

fn lcm(a: u32, b: u32) -> u32 {
    /// LCM - Least Common Multiple.
    ///	Formula: LCM(A, B) = A * B / GCD(A, B).
    return a / gcd(a, b) * b;
}

fn main() {
    println!("{}", lcm(60, 45));
}
