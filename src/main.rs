#![no_std]  // Disable the standard library
#![no_main] // Do not use default Rust entrypoints

use core::panic::PanicInfo;

mod vga_buffer;

// Don't mangle the name of this function
#[no_mangle]
pub extern "C" fn _start() -> ! {
    println!("Hello World{}", "!");
    println!("Here's a number: {}", 1.234);

    //panic!("Some panic message");

    loop {}
}

// This function is called on panic.
#[panic_handler]
fn panic(info: &PanicInfo) -> ! {
    println!("{}", info);
    loop {}
}
