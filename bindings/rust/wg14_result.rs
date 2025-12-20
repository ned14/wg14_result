use std::ffi::{CStr, c_int};
use std::mem::MaybeUninit;

include!("raw_bindings.rs");

#[doc = "A Rust Result, use to_result(WG14Result) to construct"]
pub type WG14Result<T> = Result<T, status_code_system>;

// Fill in some stuff bindgen does not
unsafe impl Send for status_code_system {}
unsafe impl Sync for status_code_system {}
impl Clone for status_code_domain_string_ref {
    fn clone(&self) -> Self {
        let mut ret = status_code_domain_string_ref::default();
        unsafe {
            status_code_domain_string_ref_copy(
                &mut ret as *mut status_code_domain_string_ref,
                self as *const status_code_domain_string_ref,
            )
        };
        ret
    }
}
impl Drop for status_code_domain_string_ref {
    fn drop(&mut self) {
        unsafe {
            status_code_domain_string_ref_destroy(self as *const status_code_domain_string_ref)
        }
    }
}
impl Clone for status_code_system {
    fn clone(&self) -> Self {
        let mut ret = status_code_system::default();
        let errcode = unsafe {
            status_code_erased_clone(
                &mut ret.base as *mut status_code_untyped,
                &self.base as *const status_code_untyped,
                status_code_domain_payload_info(self.base.domain),
            )
        };
        assert_eq!(errcode, 0);
        ret
    }
}
impl Drop for status_code_system {
    fn drop(&mut self) {
        unsafe { status_code_erased_destroy(&mut self.base as *mut status_code_untyped) }
    }
}
impl Drop for status_code_posix {
    fn drop(&mut self) {
        unsafe { status_code_erased_destroy(&mut self.base as *mut status_code_untyped) }
    }
}

impl std::fmt::Display for status_code_system {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(
            f,
            "{}",
            String::from_utf8_lossy(unsafe {
                CStr::from_ptr(status_code_message(&self.base).c_str).to_bytes()
            })
        )
    }
}

impl std::fmt::Debug for status_code_system {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(
            f,
            "{}",
            String::from_utf8_lossy(unsafe {
                CStr::from_ptr(status_code_message(&self.base).c_str).to_bytes()
            })
        )
    }
}

impl std::error::Error for status_code_system {
    fn source(&self) -> Option<&(dyn std::error::Error + 'static)> {
        None
    }
}

impl From<status_code_system> for std::io::Error {
    fn from(val: status_code_system) -> Self {
        // This is nasty, and we ought to figure out a better way of doing this
        for n in 1..200 {
            if unsafe { status_code_equivalent_errc(&val.base, n) } {
                return Self::new(Self::from_raw_os_error(n).kind(), val);
            }
        }
        Self::other(val)
    }
}

/*****************************************************************************/

#[doc = "A C compatible Result with T as successful value"]
#[repr(C)]
pub struct result_with<T> {
    pub value: MaybeUninit<T>,
    pub _flags_: result_flags,
    pub error: status_code_system,
}
impl<T> Default for result_with<T> {
    fn default() -> Self {
        Self {
            value: MaybeUninit::zeroed(),
            _flags_: result_flags::default(),
            error: status_code_system::default(),
        }
    }
}
impl<T> result_with<T> {
    #[doc = "A C Result with successful T"]
    #[inline]
    pub fn success(v: T) -> Self {
        Self {
            value: MaybeUninit::new(v),
            _flags_: result_flags {
                status: result_status_flags_result_status_flag_have_value,
            },
            error: status_code_system::default(),
        }
    }
    #[doc = "A C Result with failure"]
    #[inline]
    pub fn failure(v: status_code_system) -> Self {
        Self {
            value: MaybeUninit::zeroed(),
            _flags_: result_flags {
                status: result_status_flags_result_status_flag_have_error,
            },
            error: v,
        }
    }
    #[doc = "A C Result with failure from errno"]
    #[inline]
    pub fn failure_from_errno(v: c_int) -> Self {
        Self {
            value: MaybeUninit::zeroed(),
            _flags_: result_flags {
                status: result_status_flags_result_status_flag_have_error_error_is_errno,
            },
            error: unsafe {
                std::mem::transmute::<status_code_posix, status_code_system>(
                    status_code_posix_make(v),
                )
            },
        }
    }
    #[doc = "True if it has a value"]
    #[inline]
    pub fn has_value(&self) -> bool {
        unsafe {
            (self._flags_.status & result_status_flags_result_status_flag_have_value)
                == result_status_flags_result_status_flag_have_value
        }
    }
}

impl<T: std::fmt::Debug> std::fmt::Debug for result_with<T> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        to_result_ref(self).fmt(f)
    }
}

/*****************************************************************************/

#[doc = "From a C Result to a Rust Result"]
#[inline]
pub fn to_result<T>(res: result_with<T>) -> WG14Result<T> {
    if res.has_value() {
        Ok(unsafe { res.value.assume_init() })
    } else {
        Err(res.error)
    }
}
impl<T> From<result_with<T>> for WG14Result<T> {
    fn from(v: result_with<T>) -> Self {
        to_result(v)
    }
}

#[doc = "From a C Result to a Rust Result, by reference"]
#[inline]
pub fn to_result_ref<T>(res: &result_with<T>) -> WG14Result<&T> {
    if res.has_value() {
        Ok(unsafe { res.value.assume_init_ref() })
    } else {
        Err(res.error.clone())
    }
}

#[doc = "From a C Result to a Rust Result, by mutable reference"]
#[inline]
pub fn to_result_mut<T>(res: &mut result_with<T>) -> WG14Result<&mut T> {
    if res.has_value() {
        Ok(unsafe { res.value.assume_init_mut() })
    } else {
        Err(res.error.clone())
    }
}

#[doc = "A successful C Result"]
#[inline]
pub fn success<T>(val: T) -> result_with<T> {
    result_with::<T>::success(val)
}

#[doc = "A failed C Result from an errno value"]
#[inline]
pub fn failure_from_errno<T>(val: c_int) -> result_with<T> {
    result_with::<T>::failure_from_errno(val)
}

#[doc = "From a Rust Result to a C Result"]
pub fn from_result<T>(res: WG14Result<T>) -> result_with<T> {
    match res {
        Ok(s) => result_with::<T>::success(s),
        Err(e) => result_with::<T>::failure(e),
    }
}
impl<T> From<WG14Result<T>> for result_with<T> {
    fn from(v: WG14Result<T>) -> Self {
        from_result(v)
    }
}
