const std = @import("std");
const c = @cImport({
    @cInclude("SDL2/SDL.h");
});

pub fn main() anyerror!void {
    std.debug.print("wassup\n", .{});
}
