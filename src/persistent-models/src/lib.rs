mod helpers;
mod persistent_models;

use helpers::c_string_to_str;
use persistent_models::PersistentModels;
use std::ffi::CString;
use std::os::raw::c_char;

#[repr(C)]
pub struct PMModel {
    definition: *const c_char,
    fitness: f32,
}

#[no_mangle]
pub extern "C" fn pm_load_definition(definition: *const c_char) -> *mut PersistentModels {
    let p = PersistentModels::load_definition(c_string_to_str(definition))
        .expect("Failed to load definitions");

    Box::into_raw(Box::new(p))
}

#[no_mangle]
pub extern "C" fn pm_count(m: *mut *mut PersistentModels) -> usize {
    let models = unsafe { Box::from_raw(*m) };
    let length = models.len();
    unsafe {
        *m = Box::into_raw(models);
    }
    length
}

#[no_mangle]
pub extern "C" fn pm_get_model(m: *mut *mut PersistentModels, index: usize) -> PMModel {
    let models = unsafe { Box::from_raw(*m) };
    let model = models.get(index);

    unsafe {
        *m = Box::into_raw(models);
    }

    PMModel {
        definition: CString::new(model.definition)
            .expect("CString::new failed")
            .into_raw(),
        fitness: model.fitness,
    }
}

#[no_mangle]
pub extern "C" fn pm_free_string(s: *mut c_char) {
    unsafe {
        CString::from_raw(s);
    }
}

#[no_mangle]
pub extern "C" fn pm_free_models(m: *mut PersistentModels) {
    unsafe {
        Box::from_raw(m);
    }
}

#[no_mangle]
pub extern "C" fn pm_load_file(file_path: *const c_char) -> *mut PersistentModels {
    let p = PersistentModels::load_file(c_string_to_str(file_path))
        .expect("Failed to create persistent models object.");
    Box::into_raw(Box::new(p))
}

#[no_mangle]
pub extern "C" fn pm_save_file(m: *mut *mut PersistentModels, file_path: *const c_char) {
    let models = unsafe { Box::from_raw(*m) };

    models
        .save_file(c_string_to_str(file_path))
        .expect("Failed to save file.");

    unsafe {
        *m = Box::into_raw(models);
    }
}

#[no_mangle]
pub extern "C" fn pm_add_to_stage(
    m: *mut *mut PersistentModels,
    definition: *const c_char,
    fitness: f32,
) {
    let mut models = unsafe { Box::from_raw(*m) };

    models.add_to_stage(c_string_to_str(definition), fitness);

    unsafe {
        *m = Box::into_raw(models);
    }
}

#[no_mangle]
pub extern "C" fn pm_commit(m: *mut *mut PersistentModels, amount: usize) {
    let mut models = unsafe { Box::from_raw(*m) };

    models.commit(amount);

    unsafe {
        *m = Box::into_raw(models);
    }
}
