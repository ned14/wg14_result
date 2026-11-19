#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(non_snake_case)]

include!("wg14_result.rs");

#[cfg(test)]
mod tests {
    use super::*;

    const ERANGE: std::ffi::c_int = status_code_errc_status_code_errc_result_out_of_range;

    fn function_returning_c_result(v: i32) -> result_with<i32> {
        if v > 0 {
            return result_with::success(v);
        }
        result_with::failure_from_errno(ERANGE)
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
        assert!(res2.is_err());
        assert_eq!(res2_str, format!("Err({})", res2.as_ref().unwrap_err()));
        // The text of an errno message comes from the platform's strerror(), so
        // do not hard-code it. Compare against what std says the same errno is.
        #[cfg(unix)]
        {
            let os = std::io::Error::from_raw_os_error(ERANGE).to_string();
            let os = os
                .strip_suffix(&format!(" (os error {})", ERANGE))
                .unwrap_or(&os);
            assert_eq!(res2_str, format!("Err({})", os));
        }
    }

    #[test]
    fn test_round_trip() {
        let c = from_result::<u32>(Ok(7));
        assert!(c.has_value());
        assert_eq!(c.value(), Some(&7));
        assert_eq!(to_result(c).unwrap(), 7);

        let c = from_result::<u32>(Err(status_code_system::default()));
        assert!(!c.has_value());
        assert_eq!(c.value(), None);
        assert!(to_result(c).is_err());

        // `From` in both directions
        let c: result_with<u32> = Ok::<u32, status_code_system>(9).into();
        assert_eq!(c.value(), Some(&9));
        let r: WG14Result<u32> = c.into();
        assert_eq!(r.unwrap(), 9);
    }

    #[test]
    fn test_by_reference() {
        let mut c = result_with::success(5u32);
        assert_eq!(to_result_ref(&c).unwrap(), &5);
        assert_eq!(c.value_mut().map(|v| *v), Some(5));
        *to_result_mut(&mut c).unwrap() = 6;
        assert_eq!(to_result(c).unwrap(), 6);

        let mut c = result_with::<u32>::failure_from_errno(ERANGE);
        assert!(!c.has_value());
        assert_eq!(c.value(), None);
        assert!(to_result_ref(&c).is_err());
        assert!(to_result_mut(&mut c).is_err());
    }

    #[test]
    fn test_to_result_moves_the_value() {
        use std::cell::Cell;
        struct Tracker<'a>(&'a Cell<u32>);
        impl Drop for Tracker<'_> {
            fn drop(&mut self) {
                self.0.set(self.0.get() + 1);
            }
        }

        let drops = Cell::new(0);
        let c = result_with::success(Tracker(&drops));
        let v = to_result(c).unwrap();
        assert_eq!(drops.get(), 0, "value dropped while being moved out");
        drop(v);
        assert_eq!(drops.get(), 1, "value dropped more than once");
    }

    #[test]
    fn test_non_default_value_type() {
        // The point of MaybeUninit<T> is that T need not be Default.
        let nz = std::num::NonZeroU32::new(42).unwrap();
        assert_eq!(to_result(result_with::success(nz)).unwrap(), nz);

        let c = result_with::<std::num::NonZeroU32>::failure(status_code_system::default());
        assert!(!c.has_value());
        assert!(to_result(c).is_err());

        let c = result_with::<std::num::NonZeroU32>::failure_from_errno(ERANGE);
        assert!(!c.has_value());
        assert!(to_result(c).is_err());
    }

    #[test]
    fn test_empty_result_by_reference() {
        // A default/empty status code has no domain; cloning it must not
        // dereference a null vtable.
        let c = result_with::<u32>::default();
        assert!(!c.has_value());
        assert!(to_result_ref(&c).is_err());

        let mut c = result_with::<u32>::failure(status_code_system::default());
        assert!(!c.has_value());
        assert!(to_result_ref(&c).is_err());
        assert!(to_result_mut(&mut c).is_err());
    }

    #[test]
    fn test_drop_releases_value() {
        use std::cell::Cell;
        struct Tracker<'a>(&'a Cell<u32>);
        impl Drop for Tracker<'_> {
            fn drop(&mut self) {
                self.0.set(self.0.get() + 1);
            }
        }

        let drops = Cell::new(0);
        drop(result_with::success(Tracker(&drops)));
        assert_eq!(drops.get(), 1, "dropping a result_with leaked its value");
    }
}
