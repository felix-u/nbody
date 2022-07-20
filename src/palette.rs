use bevy::prelude::Color;

pub struct Palette {
    pub background: Color,
    pub foreground: Color,
    pub red: Color,
    pub green: Color,
    pub yellow: Color,
    pub blue: Color,
    pub magenta: Color,
    pub cyan: Color,
    pub grey: Color,
}

pub const CATPPUCCIN: Palette = Palette {
    background: Color::rgb(0.188, 0.204, 0.275),
    foreground: Color::rgb(0.776, 0.816, 0.961),
    red:        Color::rgb(0.906, 0.510, 0.518),
    green:      Color::rgb(0.655, 0.820, 0.537),
    yellow:     Color::rgb(0.898, 0.784, 0.565),
    blue:       Color::rgb(0.549, 0.667, 0.933),
    magenta:    Color::rgb(0.961, 0.722, 0.894),
    cyan:       Color::rgb(0.506, 0.784, 0.745),
    grey:       Color::rgb(0.384, 0.408, 0.502),
};
