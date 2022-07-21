use bevy::{
    core::FixedTimestep,
    math::const_vec2,
    prelude::*, 
    window::*,
    render::camera::ScalingMode,
    sprite::MaterialMesh2dBundle,
};

// amount of time that should elapse between each physics step
const TIME_STEP: f32 = 1.0 / 60.0;

// objects
const SPEED: f32 = 500.0;
// const STARTING_POSITION: const_vec2!([-0.5, -0.5]);
const SIZE: f32 = 0.2;

mod palette;
use palette::ONEBIT as PALETTE;

pub const ASPECT: f32 = 16.0/9.0;


fn main() {
    let width: f32 = 1280.0;
    App::new()
        .insert_resource(ClearColor(PALETTE.background))
        .insert_resource(WindowDescriptor {
            title: "nbody".to_string(),
            width,
            height: width / ASPECT,
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
        .add_plugins(DefaultPlugins)
        .add_startup_system(spawn_camera)
        .add_startup_system(spawn_mesh)
        .add_system(bevy::input::system::exit_on_esc_system)
        .run();
}


#[derive(Component)]
struct Object;


fn spawn_camera(mut commands: Commands) {
    let mut camera = OrthographicCameraBundle::new_2d();

    camera.orthographic_projection.top = 1.0 / ASPECT;
    camera.orthographic_projection.bottom= -1.0 / ASPECT;
    camera.orthographic_projection.right = 1.0;
    camera.orthographic_projection.left = -1.0;
    
    camera.orthographic_projection.scaling_mode = ScalingMode::None;
    
    commands.spawn_bundle(camera);
}


fn spawn_mesh(
    mut commands: Commands,
    mut meshes: ResMut<Assets<Mesh>>,
    mut materials: ResMut<Assets<ColorMaterial>>,
) {
    commands.spawn_bundle(MaterialMesh2dBundle {
        mesh: meshes.add(Mesh::from(shape::Quad::default())).into(),
        transform: Transform::default().with_scale(Vec3::splat(0.5)),
        material: materials.add(ColorMaterial::from(PALETTE.foreground)),
        ..default()
    });
}