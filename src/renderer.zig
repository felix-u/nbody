const std = @import("std");

// ANSI escape codes
// contributor <const-void> says "plunder at will" :P
// https://forum.ziggit.dev/t/how-to-clear-terminal/88/6
// (I did make changes)

// this gist was helpful too:
// https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797

// also the top comment here:
// https://stackoverflow.com/questions/4842424/list-of-ansi-color-escape-sequences

const esc = "\x1B";
const csi = esc ++ "[";

const cursor_show = csi ++ "?25h"; //h=high
const cursor_hide = csi ++ "?25l"; //l=low
const cursor_home = csi ++ "1;1H"; //1,1

const ANSIColour = struct {
    fg: []const u8,
    bg: []const u8,
};

pub const colour_default =
                      ANSIColour { .fg = "39", .bg =  "49" };
pub const colour_00 = ANSIColour { .fg = "30", .bg =  "40" };
pub const colour_01 = ANSIColour { .fg = "31", .bg =  "41" };
pub const colour_02 = ANSIColour { .fg = "32", .bg =  "42" };
pub const colour_03 = ANSIColour { .fg = "33", .bg =  "43" };
pub const colour_04 = ANSIColour { .fg = "34", .bg =  "44" };
pub const colour_05 = ANSIColour { .fg = "35", .bg =  "45" };
pub const colour_06 = ANSIColour { .fg = "36", .bg =  "46" };
pub const colour_07 = ANSIColour { .fg = "37", .bg =  "47" };
pub const colour_08 = ANSIColour { .fg = "90", .bg = "100" };
pub const colour_09 = ANSIColour { .fg = "91", .bg = "101" };
pub const colour_10 = ANSIColour { .fg = "92", .bg = "102" };
pub const colour_11 = ANSIColour { .fg = "93", .bg = "103" };
pub const colour_12 = ANSIColour { .fg = "94", .bg = "104" };
pub const colour_13 = ANSIColour { .fg = "95", .bg = "105" };
pub const colour_14 = ANSIColour { .fg = "96", .bg = "106" };
pub const colour_15 = ANSIColour { .fg = "97", .bg = "107" };
// most rendered characters will be this:
pub const fmt_default =
    csi ++ colour_default.fg ++ ";" ++ colour_default.bg ++ "m";

pub const screen_clear = csi ++ "2J";
pub const screen_buf_on = csi ++ "?1049h"; //h=high
pub const screen_buf_off = csi ++ "?1049l"; //l=low

pub const term_on = screen_buf_on ++ cursor_hide ++ cursor_home ++ screen_clear ++ fmt_default;
pub const term_off = screen_buf_off ++ cursor_show ++ "\n";














