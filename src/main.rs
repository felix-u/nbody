use bevy::{
    prelude::*,
    window::*,
    render::camera::ScalingMode,
};

mod palette;
use palette::ONEBIT as PALETTE;

pub const ASPECT: f32 = 16.0/9.0;
const DISPLAY_WIDTH: f32 = 1280.0;

const ARENA_WIDTH: f32 = 10.0;
const ARENA_HEIGHT: f32 = 10.0;


fn main() {
    App::new()
        .insert_resource(ClearColor(PALETTE.background))
        .insert_resource(WindowDescriptor {
            title: "nbody".to_string(),
            width: DISPLAY_WIDTH,
            height: DISPLAY_WIDTH / ASPECT,
            position: None,
            resize_constraints: WindowResizeConstraints::default(),
            scale_factor_override: None,
            present_mode: PresentMode::Fifo,
            resizable: false,
            decorations: true,
            cursor_locked: false,
            cursor_visible: true,
            mode: WindowMode::Windowed,
            transparent: false,
        })
        .add_startup_system(spawn_camera)
        // .add_startup_system(spawn_mesh)
        .add_startup_system(spawn_snake)
        .add_system(snake_movement)
        .add_system_set_to_stage(
            CoreStage::PostUpdate,
            SystemSet::new()
                .with_system(position_translation)
                .with_system(grid_to_screen),
        )
        .add_system(bevy::input::system::exit_on_esc_system)
        .add_plugins(DefaultPlugins)
        .run();
}


#[derive(Component)]
struct SnakeHead;
const SNAKE_HEAD_CLR: Color = PALETTE.green;

fn spawn_snake(mut commands: Commands) {
    commands
        .spawn_bundle(SpriteBundle {
            sprite: Sprite {
                color: SNAKE_HEAD_CLR,
                ..default()
            },
            // transform: Transform {
            //     scale: Vec3::new(0.02, 0.02, 1.0),
            //     ..default()
            // },
            ..default()
        })
        .insert(SnakeHead)
        .insert(Position { x: 3, y: 3 })
        .insert(Size::square(0.8))
    ;
}


const SPEED: f32 = 0.005;
fn snake_movement(keyboard_input: Res<Input<KeyCode>>, mut head_positions: Query<&mut Transform, With<SnakeHead>>) {
    for mut transform in head_positions.iter_mut() {
        if keyboard_input.pressed(KeyCode::Left) {
            transform.translation.x -= SPEED;
        }
        if keyboard_input.pressed(KeyCode::Right) {
            transform.translation.x += SPEED;
        }
        if keyboard_input.pressed(KeyCode::Down) {
            transform.translation.y -= SPEED;
        }
        if keyboard_input.pressed(KeyCode::Up) {
            transform.translation.y += SPEED;
        }
    }
}


#[derive(Component, Clone, Copy, PartialEq, Eq)]
struct Position {
    x: i32,
    y: i32,
}

#[derive(Component)]
struct Size {
    width: f32,
    height: f32,
}
impl Size {
    pub fn square(x: f32) -> Self {
        Self {
            width: x,
            height: x,
        }
    }
}


fn grid_to_screen(mut query: Query<(&Size, &mut Transform)>) {
    for (sprite_size, mut transform) in query.iter_mut() {
        transform.scale = Vec3::new(
            sprite_size.width / ARENA_WIDTH as f32,
            sprite_size.height / ARENA_HEIGHT as f32,
            1.0
        )
    }
}


fn position_translation(mut query: Query<(&Position, &mut Transform)>) {
    // fn convert(pos: f32, bound_game: f32) -> f32 {
    //     let tile_size = 1.0 / bound_game;
    //     pos / bound_game - 0.5 + (tile_size / 2.0)
    // }
    // for (pos, mut transform) in query.iter_mut() {
    //     transform.translation = Vec3::new(
    //         convert(pos.x as f32, ARENA_WIDTH as f32),
    //         convert(pos.y as f32, ARENA_HEIGHT as f32),
    //         0.0,
    //     );
    // }
}


fn spawn_camera(mut commands: Commands) {
    let mut camera = OrthographicCameraBundle::new_2d();
    camera.orthographic_projection.top = 1.0 / ASPECT;
    camera.orthographic_projection.bottom= -1.0 / ASPECT;
    camera.orthographic_projection.right = 1.0;
    camera.orthographic_projection.left = -1.0;
    camera.orthographic_projection.scaling_mode = ScalingMode::None;
    commands.spawn_bundle(camera);
}
