# shell.nix

{ pkgs ? import <nixpkgs> {} }:
with pkgs; mkShell {
  nativeBuildInputs = [
    # pkgconfig
    gcc lld # To use lld linker
    SDL2
  ];
  buildInputs = [
    # xorg.libX11 xorg.libXcursor xorg.libXrandr xorg.libXi # To use x11 feature
    # libxkbcommon wayland # To use wayland feature
    SDL2
  ];
}
