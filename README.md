# Pipe to Type

An addon for fcitx5 to commit string via named pipe.

## Install

```sh
./install.sh
```

It installs to `~/.local` in a peculiar way that is most likely not officially supported.
Read and edit the script to tailor it to your needs.

## Usage

```sh
echo -n Hello World > $XDG_RUNTIME_DIR/fcitx5-pipe-to-type
```

## See also

- [fcitx-dbus-commit-string](https://github.com/amosbird/fcitx-dbus-commit-string) (for fcitx4)
