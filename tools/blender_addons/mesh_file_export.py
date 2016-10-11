import bpy
import mathutils
import struct

def vector_to_list(v, swap_y_z):
    order = [0, 2, 1] if swap_y_z else [0, 1, 2]
    return [v[i] for i in order]

matrix_swap_y_z = mathutils.Matrix()
matrix_swap_y_z[0][0:4] = 1.0, 0.0, 0.0, 0.0
matrix_swap_y_z[1][0:4] = 0.0, 0.0, 1.0, 0.0
matrix_swap_y_z[2][0:4] = 0.0, 1.0, 0.0, 0.0
matrix_swap_y_z[3][0:4] = 0.0, 0.0, 0.0, 1.0

def collect_mesh_data(obj, smooth, swap_y_z):
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
            vertices_order = [0, i, i + 1] if not swap_y_z else [0, i + 1, i]
            for j in vertices_order:
                vert_i = f.vertices[j]
                loop_i = f.loop_indices[j]
                v_out = {}
                v_out['pos'] = vector_to_list(obj.data.vertices[vert_i].co, swap_y_z)
                if smooth:
                    v_out['norm'] = vector_to_list(obj.data.vertices[vert_i].normal, swap_y_z)
                else:
                    v_out['norm'] = vector_to_list(f.normal, swap_y_z)
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
                pose.append(b.matrix_basis.copy() if not swap_y_z else (matrix_swap_y_z * b.matrix_basis* matrix_swap_y_z))
            action.append([frame, pose])
        actions.append([act.frame_range.to_tuple(), action])
        
    return mesh

def write_mesh_data(context, filepath, mesh, normal, color, uv, animation):
    print("running write_mesh_data...")
    f = open(filepath, 'wb')
    # magic number
    f.write(struct.pack('BBBB', 0x4d, 0x45, 0x53, 0x48))
    # position
    attrib = 1
    # normal
    attrib = attrib | (2 if normal else 0)
    # color
    attrib = attrib | (4 if color else 0)
    # uv
    output_uv = uv and mesh['has_uv']
    attrib = attrib | (8 if output_uv else 0)
    # animation
    output_anim = animation and 'bones' in mesh
    attrib = attrib | (16 if output_anim else 0) 
    f.write(struct.pack('I', attrib))
    
    # number of vertex
    f.write(struct.pack('I', len(mesh['vertices'])))
    
    # number of bones
    f.write(struct.pack('I', 0 if not 'bones' in mesh else len(mesh['bones'])))
    
    # number of actions
    f.write(struct.pack('I', 0 if not 'actions' in mesh else len(mesh['actions'])))
    
    for v in mesh['vertices']:
        #f.write("v %f %f %f" % v['pos'].to_tuple())
        f.write(struct.pack('fff', *(v['pos'])))
        if normal:
            #f.write(" %f %f %f" % v['norm'].to_tuple())
            f.write(struct.pack('fff', *(v['norm'])))
        if color:
            if mesh['has_color']:
                #f.write(" %f %f %f" % tuple(v['color']))
                f.write(struct.pack('fff', *(v['color'])))
            else:
                f.write(struct.pack('fff', 0.7, 0.7, 0.7))
        if output_uv:
            #f.write(" %f %f" % tuple(v['uv']))
            f.write(struct.pack('ff', *(v['uv'])))
        if output_anim:
            n_group = 0 if not 'bone' in v else len(v['bone'])
            for i in range(0, min(n_group, 4)):
                # f.write(" %d %f" % tuple(g))
                f.write(struct.pack('If', *(v['bone'][i])))
            if n_group < 4:
                for i in range(n_group, 4):
                    f.write(struct.pack('If', 0, 0.0))
    
    if not output_anim:
        f.close()
        return
    
    for b in mesh['bones']:
        #f.write("b %d\n" % b)
        f.write(struct.pack('i', b))
    
    if not 'actions' in mesh:
        f.close()
        return
    
    for a in mesh['actions']:
        #f.write("action (%f - %f)\n" % a[0])
        f.write(struct.pack('III', int(a[0][0]), int(a[0][1]), len(a[1])))
        for frame in a[1]:
            #f.write("frame %d\n" % frame[0])
            f.write(struct.pack('I', frame[0]))
            for b in frame[1]:
                t = b.to_translation()
                #f.write('\tT %f %f %f\n' % t.to_tuple())
                f.write(struct.pack('fff', *t))
                q = b.to_quaternion()
                #f.write('\tR %f %f %f %f\n' % (q.w, q.x, q.y, q.z))
                f.write(struct.pack('ffff', *q))
                s = b.to_scale()
                #f.write('\tS %f %f %f\n' % s.to_tuple())
                f.write(struct.pack('fff', *s))
        
    f.close()
    return


# ExportHelper is a helper class, defines filename and
# invoke() function which calls the file selector.
from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty, BoolProperty, EnumProperty
from bpy.types import Operator

# axis = (('POS_X', "+X", "use +X as export to this axis"),
#         ('NEG_X', "-X", "use -X as export to this axis"),
#         ('POS_Y', "+Y", "use +Y as export to this axis"),
#         ('NEG_Y', "-Y", "use -Y as export to this axis"),
#         ('POS_Z', "+Z", "use +Z as export to this axis"),
#         ('NEG_Z', "-Z", "use -Z as export to this axis"))

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
    smooth_normal = BoolProperty(
            name="Smooth Normals",
            description="Smooth Normals",
            default=False,
            )

    export_normal = BoolProperty(
            name="Export Normals",
            description="Export Normals",
            default=True,
            )

    export_color = BoolProperty(
            name="Export Colors",
            description="Export Colors",
            default=True,
            )

    export_uv = BoolProperty(
            name="Export UVs",
            description="Export UVs",
            default=True,
            )

    export_anim = BoolProperty(
            name="Export Animations",
            description="Export Animations",
            default=True,
            )

    swap_y_z = BoolProperty(
            name="Swap Y and Z",
            description="Swap Y axis and Z axis",
            default=True,
            )

    # z_pos = EnumProperty(
    #         name="+Z",
    #         description="axis to export",
    #         items=axis,
    #         default='POS_Y',
    #         )

    # y_pos = EnumProperty(
    #         name="+Y",
    #         description="axis to export",
    #         items=axis,
    #         default='POS_Z',
    #         )

    def execute(self, context):
        scene = bpy.context.scene
        for obj in bpy.data.objects:
            if obj.type == 'MESH':
                mesh = collect_mesh_data(obj, self.smooth_normal, self.swap_y_z)
                write_mesh_data(context, self.filepath, mesh, self.export_normal, self.export_color, self.export_uv, self.export_anim)
                break
        
        return {'FINISHED'}


# Only needed if you want to add into a dynamic menu
def menu_func_export(self, context):
    self.layout.operator(ExportMeshFileData.bl_idname, text="Mesh File (.mesh)")


def register():
    bpy.utils.register_class(ExportMeshFileData)
    bpy.types.INFO_MT_file_export.append(menu_func_export)


def unregister():
    bpy.utils.unregister_class(ExportMeshFileData)
    bpy.types.INFO_MT_file_export.remove(menu_func_export)


if __name__ == "__main__":
    #unregister()
    register()
    bpy.ops.export_mesh.mesh_file('INVOKE_DEFAULT')
