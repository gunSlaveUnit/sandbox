use std::collections::HashMap;

fn two_sum(nums: Vec<i32>, target: i32) -> Vec<i32> {
    let mut m = HashMap::new();

    for i in 0..nums.len() {
        let diff = target - nums[i];

        if m.contains_key(&diff) {
            return vec![m[&diff], i as i32];
        }

        m.insert(nums[i], i as i32);
    }

    vec![]
}

fn main() {
    let nums = vec![2, 7, 11, 15];
    let target = 9;
    let result = two_sum(nums, target);
}