import bpy

def collect_mesh_data(obj, smooth):
    if not 'type' in dir(obj):
        return None
    if obj.type != 'MESH':
        return None
    scene = bpy.context.scene
    mesh = {}
    vertices = []
    mesh['name'] = obj.name
    mesh['vertices'] = vertices
    mesh['has_color'] = False
    color_layer = None
    if obj.data.vertex_colors is not None:
        for layer in obj.data.vertex_colors:
            if layer.active:
                mesh['has_color'] = True
                color_layer = layer
            break
    mesh['has_uv'] = obj.data.uv_layers.active is not None
        
    for f in obj.data.polygons:
        for i in range(1, len(f.vertices) - 1):
            for j in [0, i, i + 1]:
                vert_i = f.vertices[j]
                loop_i = f.loop_indices[j]
                v_out = {}
                v_out['pos'] = obj.data.vertices[vert_i].co
                if smooth:
                    v_out['norm'] = obj.data.vertices[vert_i].normal
                else:
                    v_out['norm'] = f.normal
                if mesh['has_color']:
                    v_out['color'] = color_layer.data[loop_i].color
                if mesh['has_uv']:
                    v_out['uv'] = obj.data.uv_layers.active.data[loop_i].uv
                if obj.data.vertices[vert_i].groups is None:
                    continue
                v_out['bone'] = []
                for g in obj.data.vertices[vert_i].groups:
                    name = obj.vertex_groups[g.group].name # not sure gid == index
                    v_out['bone'].append([name, g.weight])
                vertices.append(v_out)
                
    armature = obj.find_armature()
    if not armature:
        return mesh
    bones = []
    actions = []
    mesh['bones'] = bones
    mesh['actions'] = actions
    name_table = {}
    for i in range(0, len(armature.data.bones)):
        name_table[armature.data.bones[i].name] = i
    for i in range(0, len(armature.data.bones)):
        if not armature.data.bones[i].parent:
            bones.append(-1)
        else:
            bones.append(name_table[armature.data.bones[i].parent.name])
    for v in vertices:
        for g in v['bone']:
            g[0] = name_table[g[0]]
    
    for act in bpy.data.actions:
        armature.animation_data.action = act
        action = []
        scene.frame_set(0)
        while {'FINISHED'} == bpy.ops.screen.keyframe_jump():
            frame = scene.frame_current
            scene.frame_set(frame)
            pose = []
            for b in armature.pose.bones:
                pose.append(b.matrix.copy())
            action.append([frame, pose])
        actions.append(action)
        
    return mesh

def write_mesh_data(context, filepath, mesh, normal, color, uv, animation):
    print("running write_mesh_data...")
    f = open(filepath, 'w', encoding='utf-8')
    
    for v in mesh['vertices']:
        f.write("v %f %f %f" % v['pos'].to_tuple())
        if normal:
            f.write(" %f %f %f" % v['norm'].to_tuple())
        if color and mesh['has_color']:
            f.write(" %f %f %f" % tuple(v['color']))
        if uv and mesh['has_uv']:
            f.write(" %f %f" % tuple(v['uv']))
        if animation and 'bone' in v:
            for g in v['bone']:
                f.write(" %d %f" % tuple(g))
        f.write("\n")
        
    f.write("\n")
    
    if not animation or not 'bones' in mesh:
        f.close()
        return
    
    for b in mesh['bones']:
        f.write("b %d\n" % b)
    
    f.write("\n")
    
    for a in mesh['actions']:
        f.write("action\n")
        for frame in a:
            f.write("frame %d\n" % frame[0])
            for b in frame[1]:
                for r in range(0, 4):
                    f.write('\t%f %f %f %f\n' % b.row[r].to_tuple())
                f.write('\n')
        
    f.close()
    return


# ExportHelper is a helper class, defines filename and
# invoke() function which calls the file selector.
from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty, BoolProperty, EnumProperty
from bpy.types import Operator


class ExportMeshFileData(Operator, ExportHelper):
    """This appears in the tooltip of the operator and in the generated docs"""
    bl_idname = "export_mesh.mesh_file"  # important since its how bpy.ops.import_test.some_data is constructed
    bl_label = "Export Mesh(.mesh)"

    # ExportHelper mixin class uses this
    filename_ext = ".mesh"

    filter_glob = StringProperty(
            default="*.mesh",
            options={'HIDDEN'},
            maxlen=255,  # Max internal buffer length, longer would be clamped.
            )

    # List of operator properties, the attributes will be assigned
    # to the class instance from the operator settings before calling.
    export_normal = BoolProperty(
            name="Export Normals",
            description="Export Normals",
            default=True,
            )

    export_color = BoolProperty(
            name="Export Colors",
            description="Export Colors",
            default=False,
            )

    export_uv = BoolProperty(
            name="Export UVs",
            description="Export UVs",
            default=False,
            )

    export_anim = BoolProperty(
            name="Export Animations",
            description="Export Animations",
            default=False,
            )

    #type = EnumProperty(
    #        name="Example Enum",
    #        description="Choose between two items",
    #        items=(('OPT_A', "First Option", "Description one"),
    #               ('OPT_B', "Second Option", "Description two")),
    #        default='OPT_A',
    #        )

    def execute(self, context):
        scene = bpy.context.scene
        for obj in bpy.data.objects:
            if obj.type == 'MESH':
                mesh = collect_mesh_data(obj, False)
                write_mesh_data(context, self.filepath, mesh, self.export_normal, self.export_color, self.export_uv, self.export_anim)
                break
        
        return {'FINISHED'}


# Only needed if you want to add into a dynamic menu
def menu_func_export(self, context):
    self.layout.operator(ExportMeshFileData.bl_idname, text="Mesh File Export Operator")


def register():
    bpy.utils.register_class(ExportMeshFileData)
    bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_class(ExportMeshFileData)
    bpy.types.INFO_MT_file_export.remove(menu_func_export)


if __name__ == "__main__":
    register()

    # test call
    bpy.ops.export_mesh.mesh_file('INVOKE_DEFAULT')
