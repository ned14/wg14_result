#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]

include!("wg14_result.rs");

#[cfg(test)]
mod tests {
    use super::*;

    fn function_returning_c_result(v: i32) -> result_with<i32> {
        if v > 0 {
            return result_with::success(v);
        }
        result_with::failure_from_errno(
            status_code_errc_status_code_errc_result_out_of_range, /* ERANGE */
        )
    }

    fn function_returning_rust_result(v: i32) -> WG14Result<String> {
        // Rust's TRY operator works just fine
        let x = to_result(function_returning_c_result(v))?;
        Ok(x.to_string())
    }

    #[test]
    fn test_result_works() {
        let res1 = function_returning_rust_result(42);
        let res2 = function_returning_rust_result(-42);
        let res1_str = format!("{:?}", res1);
        let res2_str = format!("{:?}", res2);
        println!("test(42) returns {}", res1_str);
        println!("test(-42) returns {}", res2_str);
        assert_eq!(res1_str, "Ok(\"42\")");
        assert_eq!(res2_str, "Err(Result too large)");
    }
}
