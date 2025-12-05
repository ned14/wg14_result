include! {"raw_bindings.rs"}

use std::ffi::CStr;

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
        Self::new(std::io::ErrorKind::Other, val)
    }
}

/*****************************************************************************/

#[doc = "A C compatible Result with T as successful value"]
#[repr(C)]
pub struct result_with<T> {
    pub value: T,
    pub _flags_: result_flags,
    pub error: status_code_system,
}
impl<T> Default for result_with<T> {
    fn default() -> Self {
        let mut s = ::std::mem::MaybeUninit::<Self>::uninit();
        unsafe {
            ::std::ptr::write_bytes(s.as_mut_ptr(), 0, 1);
            s.assume_init()
        }
    }
}
impl<T: Default> result_with<T> {
    #[doc = "A C Result with successful T"]
    #[inline]
    pub fn success(v: T) -> Self {
        result_with::<T> {
            value: v,
            _flags_: result_flags {
                status: result_status_flags_result_status_flag_have_value,
            },
            error: status_code_system::default(),
        }
    }
    #[doc = "A C Result with failure"]
    #[inline]
    pub fn failure(v: status_code_system) -> Self {
        result_with::<T> {
            value: T::default(),
            _flags_: result_flags {
                status: result_status_flags_result_status_flag_have_error,
            },
            error: v,
        }
    }
    #[doc = "A C Result with failure from errno"]
    #[inline]
    pub fn failure_from_errno(v: ::std::os::raw::c_int) -> Self {
        result_with::<T> {
            value: T::default(),
            _flags_: result_flags {
                status: result_status_flags_result_status_flag_have_error_error_is_errno,
            },
            error: unsafe {
                ::std::mem::transmute::<status_code_posix, status_code_system>(
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

impl<T: Default + Copy + std::fmt::Debug> std::fmt::Debug for result_with<T> {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        if self.has_value() {
            let v: WG14Result<T> = Ok(self.value);
            return v.fmt(f);
        } else {
            let v: WG14Result<T> = Err(self.error.clone());
            return v.fmt(f);
        }
    }
}

/*****************************************************************************/

#[doc = "From a C Result to a Rust Result"]
#[inline]
pub fn to_result<T: Default>(res: result_with<T>) -> WG14Result<T> {
    if res.has_value() {
        return Ok(res.value);
    }
    Err(res.error)
}

#[doc = "A successful C Result"]
#[inline]
pub fn success<T: Default>(val: T) -> result_with<T> {
    result_with::<T>::success(val)
}

#[doc = "A failed C Result from an errno value"]
#[inline]
pub fn failure_from_errno<T: Default>(val: ::std::os::raw::c_int) -> result_with<T> {
    result_with::<T>::failure_from_errno(val)
}
