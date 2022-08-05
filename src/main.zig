const std = @import("std");
const print = std.debug.print;
const sdl = @import("SDL2");

pub fn main() anyerror!void {

    // initialise SDL2
    // if (sdl.SDL_Init(sdl.SDL_INIT_EVERYTHING) != 0) {
    //     print("SDL_Init Error: {s}\n", .{sdl.SDL_GetError()});
    //     std.os.exit(69); // UNAVAILABLE
    // }
    // defer sdl.SDL_Quit();

    print("wassmup", .{});
}
