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

// onedark
impl Default for Palette {
    fn default() -> Palette {
        Palette {
            background: Color::rgb(0.118, 0.129, 0.153),
            foreground: Color::rgb(0.671, 0.698, 0.749),
            red:        Color::rgb(0.878, 0.424, 0.459),
            green:      Color::rgb(0.596, 0.765, 0.475),
            yellow:     Color::rgb(0.820, 0.604, 0.400),
            blue:       Color::rgb(0.380, 0.686, 0.937),
            magenta:    Color::rgb(0.776, 0.471, 0.867),
            cyan:       Color::rgb(0.337, 0.714, 0.761),
            grey:       Color::rgb(0.443, 0.490, 0.576),
        }
    }
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

pub const 
