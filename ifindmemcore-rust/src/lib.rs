#![feature(rustc_private)]

use std::io::{self, Write};
use mach::task::{task_for_pid, mach_task_self};
use mach::kern_return::KERN_SUCCESS;
use std::io::{self, Write};
use mach::task::{task_for_pid, mach_task_self};
use mach::kern_return::KERN_SUCCESS;

fn interact(pid: i32, task: mach_port_t) -> io::Result<()> {
    let mut input = String::new();
    let mut args: Vec<String> = Vec::new();

    println!("To list RevelariOS commands, type 'help'");

    loop {
        print!(">> ");
        io::stdout().flush()?;
        io::stdin().read_line(&mut input)?;

        args.clear();
        for word in input.trim().split_whitespace() {
            args.push(word.to_string());
        }

        match args[0].as_str() {
            "help" => {
                println!("List of commands:..."); // Simplified for brevity
            },
            "exit" | "quit" => {
                break;
            },
            "pid" => {
                if args.len() > 1 {
                    let new_pid: i32 = args[1].parse().unwrap_or_else(|_| {
                        println!("Invalid PID");
                        0
                    });
                    if new_pid!= 0 {
                        let kret = task_for_pid(mach_task_self(), new_pid, &mut task);
                        if kret == KERN_SUCCESS {
                            println!("Changed pid to {}", new_pid);
                        } else {
                            println!("Could not change PID");
                        }
                    }
                } else {
                    println!("Current PID: {}", pid);
                }
            },
            _ => {
                println!("Invalid Command");
            }
        }

        input.clear();
    }

    Ok(())
}

fn main() -> io::Result<()> {
    let mut pid: i32 = 0;
    let mut task: mach_port_t = 0;

    println!("Welcome to RevelariOS!");

    // Check if running as root
    if!std::env::var("USER").unwrap_or_default().contains("root") {
        println!("Run RevelariOS as root.");
        return Ok(());
    }

    if std::env::args().len() > 1 {
        pid = std::env::args().nth(1).unwrap().parse().unwrap_or_else(|_| {
            println!("Invalid PID");
            0
        });
    } else {
        println!("PID to attach: ");
        let mut input = String::new();
        io::stdin().read_line(&mut input)?;
        pid = input.trim().parse().unwrap_or_else(|_| {
            println!("Invalid PID");
            0
        });
    }

    let kret = task_for_pid(mach_task_self(), pid, &mut task);
    if kret!= KERN_SUCCESS {
        println!("Couldn't obtain task_for_pid({}). Do you have proper entitlements?", pid);
        return Ok(());
    } else {
        println!("Obtained task_for_pid({})", pid);
    }

    interact(pid, task)?;

    Ok(())
}