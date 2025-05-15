use std::io;
use std::fs::File;
use std::io::Read;
use colored::*;


fn print_jpeg(buffer: &mut [u8]) {
    // change to array since fixed at 16
    let mut v = Vec::new();
    let mut marker = false;

    println!();
    for _ in 0..49 {
        print!("_");
    }
    println!();
    let mut count = 0;

    for byte in buffer {

        match count {
            8 => {
                print!("| ");
                count += 1;
            },
            16 => {
                for c in &v {
                    print!("{}", c);
                }
                v.clear();
                println!();
                count = 1;
            }
            _ => count += 1,
        }
        match *byte {
            0 => {
                print!("{} ", "00".truecolor(85, 86, 93));
                v.push(".".truecolor(85, 86, 93));
            },
            0x01..=0x0f => {
                print!("{} ", format!("0{:x}", byte).green());
                v.push("\'".green());
            },
            0xff => {
                print!("{} ", "ff".red());
                v.push("*".red());
                marker = true;
                continue;
            },
            31..=126 => {
                print!("{} ", format!("{:x}", byte).cyan());
                v.push(format!("{}", *byte as char).cyan());
            },
            _ => {
                if marker {
                    print!("{} ", format!("{:x}", byte).red());
                    v.push("*".red());
                } else {
                    print!("{} ", format!("{:x}", byte).yellow());
                    v.push("*".yellow());
                }

            },
        }
        marker = false;
    }
    println!();
}

fn print_jpeg_layout(buffer: &mut [u8]) {
    let mut i = 0;
    let mut marker = false;
    while i < buffer.len() {
        if marker {
            if buffer[i] == 0 {
                marker = false;
                continue;
            }
            print!("[ {} {:x} ] ", "ff", buffer[i]);
            match buffer[i] {
                0xd8 => println!("Start of image.."),
                0xe0..=0xef  => {
                    print!("App {:2}: ", buffer[i] - 0xe0);
                    i += 3;
                    while 31 < buffer[i] && buffer[i] < 127{
                            print!("{}", buffer[i] as char);
                        i += 1;
                    }
                    println!();
                },
                0xc0 => println!("Baseline DCT"),
                0xdb => println!("Define Quantization Table"),
                0xc4 => println!("Define Huffman Table"),
                0xda => println!("Start of scan"),
                _    => println!("not implemented"),
            }
            marker = false;
        }
        if buffer[i] == 0xff {
            marker = true;
        }
        i+=1;
    }
}

fn main() -> io::Result<()> {
    let mut f = File::open("x.txt")?;
    let mut buffer = [0; 4000];

    f.read(&mut buffer)?;

    print_jpeg_layout(&mut buffer);
    print_jpeg(&mut buffer);

    Ok(())
}
