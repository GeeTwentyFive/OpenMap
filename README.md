W.I.P. general-purpose 3D & 2D map editor

Register your own MapObjects(models & sprites),
instantiate and manipulate them in 3D space,
then export them to JSON (default) or to a custom format via a custom export function!

## Controls:
- Right mouse button +
  - Mouse move = look
  - WASD/Space/Ctrl = move
- Scroll = change camera move speed
- Middle mouse button = select/deselect existing MapObject
- Left mouse button = move gizmo / interact with GUI
- DEL = delete selected MapObject

## Configuration:
`config.chai`:
```
// MapObject registration example:
AddMapObject("NAME", MODEL, "PATH_TO_MODEL", "OPTIONAL DEFAULT EXTRA DATA");
AddMapObkect("NAME", SPRITE, "PATH_TO_SPRITE", "OPTIONAL DEFAULT EXTRA DATA");
// ...

// Custom export example (if function definition missing: exports as JSON):
def export(map_objects) {

        var out = "";

        for (map_object : map_objects) {
                out += map_object.name + "\n";
                out += map_object.pos[0] + " " + map_object.pos[1] + " " + map_object.pos[2] + "\n";
                out += map_object.rot[0] + " " + map_object.rot[1] + " " + map_object.rot[2] + "\n";
                out += map_object.scale[0] + " " + map_object.scale[1] + " " + map_object.scale[2] + "\n";
                out += map_object.extra_data + "\n";
                out += "\n";
        }

        return out;

}
```
