const std = @import("std");
const io = std.io;
const print = std.debug.print;
const c = @cImport({
    @cInclude("sys/ioctl.h");
    @cInclude("stdio.h");
});
const render = @import("renderer");


pub fn main() anyerror!void {

    // GPA if debugging/testing, c_allocator otherwise
    const mode = @import("builtin").mode;
    var gpa = if (mode == .Debug) std.heap.GeneralPurposeAllocator(.{}){} else void{};
    defer if (mode == .Debug) { _ = gpa.deinit(); };
    const alloc = if (mode == .Debug) gpa.allocator() else std.heap.c_allocator;

    var win: c.winsize = undefined;
    _ = c.ioctl(0, c.TIOCGWINSZ, &win);
    // print("Terminal dimensions are width:{d} x height:{d}\n", .{win.ws_col, win.ws_row});

    // TODO: actually work out how much memory the whole buffer will take
    //       after ANSI escape codes + unicode char
    var buf = try alloc.alloc(u8, win.ws_col*win.ws_row);
    defer alloc.free(buf);

    var stdout_writer = io.getStdOut().writer();
    try stdout_writer.writeAll(render.term_on);
    var i: u16 = 0;
    while (i < (win.ws_col * win.ws_row)) : (i += 1) {
        for (buf) |*char| char.* = ' ';
        buf[i/100] = 'X';
        try stdout_writer.writeAll(buf);
    }
    try stdout_writer.writeAll(render.term_off);

    // var buf_stream = io.fixedBufferStream(&buf);
    // try buf_stream.writer().print("Hello, World", .{});

}
