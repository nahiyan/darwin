use std::ffi::CStr;
use std::os::raw::c_char;

pub fn c_string_to_str(c_string: *const c_char) -> &'static str {
    unsafe { CStr::from_ptr(c_string) }
        .to_str()
        .expect("Failed to convert C String.")
}
