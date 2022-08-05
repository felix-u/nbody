const std = @import("std");
const print = std.debug.print;
const c = @cImport({
    @cInclude("SDL2/SDL.h");
});

pub fn main() anyerror!void {

    // initialise SDL2
    if (c.SDL_Init(c.SDL_INIT_EVERYTHING) != 0) {
        print("SDL_Init Error: {s}\n", .{c.SDL_GetError()});
        std.os.exit(69); // UNAVAILABLE
    }
    defer c.SDL_Quit();

    print("wassmup", .{});
}
