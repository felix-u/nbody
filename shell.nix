# shell.nix

{ pkgs ? import <nixpkgs> {} }:
let
    pkgs-unstable = import <nixos-unstable> { config = { allowUnfree = true; }; };
in
with pkgs; mkShell {
  nativeBuildInputs = [
    # pkgconfig
    gcc lld # To use lld linker
  ];
  buildInputs = [
    # xorg.libX11 xorg.libXcursor xorg.libXrandr xorg.libXi # To use x11 feature

    # To use wayland feature
    egl-wayland libdecor libxkbcommon wayland wayland-scanner wayland-utils
    wayland-protocols pkgs-unstable.SDL2
  ];
}
