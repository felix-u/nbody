const std = @import("std");
const print = std.debug.print;
const c = @cImport({
    @cInclude("sys/ioctl.h");
    @cInclude("stdio.h");
});
// const c = std.c;


pub fn main() anyerror!void {
    var win: c.winsize = undefined;
    _ = c.ioctl(0, c.TIOCGWINSZ, &win);
    print("Terminal dimensions are width:{d} x height:{d}\n", .{win.ws_col, win.ws_row});
}
