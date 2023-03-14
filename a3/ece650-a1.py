import sys

class ParseExceptionError(Exception):
    def __init__(self, value):
        self._value = value
    def __str__(self):
        return str(self._value) 
        
# https://stackoverflow.com/a/20677983
#function for intersection points
def line_intersection(line1, line2):
    xdiff = (line1[0][0] - line1[1][0], line2[0][0] - line2[1][0])
    ydiff = (line1[0][1] - line1[1][1], line2[0][1] - line2[1][1])

    def det(a, b):
        return a[0] * b[1] - a[1] * b[0]

    div = det(xdiff, ydiff)
    if div == 0:
        return 0.0, 0.0
    d = (det(*line1), det(*line2))
    x = det(d, xdiff) / div
    y = det(d, ydiff) / div
    
    if x==0:
        x=0.0
    if y==0:
        y=0.00

    return x, y

def float_print(num):
    return "{0:.2f}".format(num)

# assumes line segments are stored in the format [(x0,y0),(x1,y1)]
# https://stackoverflow.com/a/62625458
# https://algorithmtutor.com/Computational-Geometry/Check-if-two-line-segment-intersect/
def intersects(s0, s1):
    dx0 = s0[1][0] - s0[0][0]
    dx1 = s1[1][0] - s1[0][0]
    dy0 = s0[1][1] - s0[0][1]
    dy1 = s1[1][1] - s1[0][1]
    p0 = dy1 * (s1[1][0] - s0[0][0]) - dx1 * (s1[1][1] - s0[0][1])
    p1 = dy1 * (s1[1][0] - s0[1][0]) - dx1 * (s1[1][1] - s0[1][1])
    p2 = dy0 * (s0[1][0] - s1[0][0]) - dx0 * (s0[1][1] - s1[0][1])
    p3 = dy0 * (s0[1][0] - s1[1][0]) - dx0 * (s0[1][1] - s1[1][1])
    return (p0 * p1 <= 0) & (p2 * p3 <= 0)

#converting all kinds of input into float
def convert_to_float(street_map):
    for key in street_map:
        points = street_map[key]
        float_points = []
        for point in points:
            float_points.append((float(point[0]), float(point[1])))
        street_map[key] = float_points
    return street_map


vertexes = []
lines_to_vertex_map = {}

#function to add line to vertex map
def add_to_vertex_map(vertex, line1, line2):
    
    line1 = sorted(line1, key=lambda element: (element[0], element[1]))
    line2 = sorted(line2, key=lambda element: (element[0], element[1]))
    if str(line1) not in lines_to_vertex_map:
        lines_to_vertex_map[str(line1)] = [vertex]
    else:
        lines_to_vertex_map[str(line1)].append(vertex)
        lines_to_vertex_map[str(line1)] = list(set(lines_to_vertex_map[str(line1)]))

    if str(line2) not in lines_to_vertex_map:
        lines_to_vertex_map[str(line2)] = [vertex]
    else:
        lines_to_vertex_map[str(line2)].append(vertex)
        lines_to_vertex_map[str(line2)] = list(set(lines_to_vertex_map[str(line2)]))

#function to add points to vertex
def add_to_vertexes(points):
    for point in points:
        if point not in vertexes:
            vertexes.append((point[0], point[1]))

#function to calculate distance
def distance(point1, point2):
    x1, x2 = point1
    y1, y2 = point2

    distance = float(((x2 - x1) ** 2 - (y2 - y1) ** 2))
    if distance > 0:
        distance = distance ** (0.5)
    else:
        distance = (distance * (-1)) ** (0.5)
    return distance

# function for sorting edges
def calculate_edges_by_sorting(lines_to_vertex_map):
    edges = []
    for key in lines_to_vertex_map:
        points = eval(key)
        intercepts = lines_to_vertex_map[key]
        points.extend(intercepts)
        points = list(set(points))
        # points.sort()
        # https://stackoverflow.com/a/23961489
        points = sorted(points, key=lambda element: (element[0], element[1]))
        current_edges = [[points[i], points[i + 1]] for i in range(len(points) - 1)]
        edges.extend(current_edges)
    return edges

#function to generate vertices
def generate_vertices(street_map_copy):
    street_map = convert_to_float(street_map_copy)
    street_names = list(street_map.keys())
    for i in range(len(street_names)):
        for j in range(i + 1, len(street_names)):
            street_one_points = street_map[street_names[i]]
            street_two_points = street_map[street_names[j]]
            for k in range(len(street_one_points) - 1):
                point_one, point_two = street_one_points[k], street_one_points[k + 1]
                for l in range(len(street_two_points) - 1):
                    point_three, point_four = (
                        street_two_points[l],
                        street_two_points[l + 1],
                    )
                    does_intersect = intersects(
                        (point_one, point_two), (point_three, point_four)
                    )
                    if does_intersect:
                        vertex = line_intersection(
                            (point_one, point_two), (point_three, point_four)
                        )
                        current_vertices = [
                            vertex,
                            point_one,
                            point_two,
                            point_three,
                            point_four,
                        ]
                        add_to_vertexes(current_vertices)
                        add_to_vertex_map(
                            vertex, [point_one, point_two], [point_three, point_four]
                        )
                    # print("\n")

#function to print vertices
def print_vertices(vertex_to_number):
    # print("V", len(vertex_to_number))
    # print("debug a1 V")
    sys.stdout.write('V {}\n'.format(len(vertex_to_number)))
    sys.stdout.flush()

#function to print edges
def print_edges(edges_to_number):
    # print("E {")
    # print("debug a1 E")

    sys.stdout.write('E {')
    sys.stdout.flush()
    for i, edge in enumerate(edges_to_number):
        num1, num2 = edge
        if i == len(edges_to_number) - 1:
            sys.stdout.write(str('<') + str(int(num1)) + str(',') + str(int(num2)) + str('>'))
            sys.stdout.flush()
            # print(f"<{int(num1)},{int(num2)}>", end='')
        else:
            sys.stdout.write(str('<') + str(int(num1)) + str(',') + str(int(num2)) + str('>') + str(','))
            sys.stdout.flush()
            # print(f"<{int(num1)},{int(num2)}>,", end='')
    # print("}")
    # print("meena", flush=True)
    sys.stdout.write('}\n')
    sys.stdout.flush()


#function for vertex and edges numbering
def numbering(vertices, edges):
    number_to_vertex = {}
    vertex_to_number = {}
    for i in range(len(vertices)):
        vertex = vertices[i]
        number_to_vertex[i + 1] = vertex
        vertex_to_number[vertex] = i + 1

    edges_to_number = []
    for edge in edges:
        vertex1, vertex2 = edge
        edge_num = (vertex_to_number[vertex1], vertex_to_number[vertex2])
        edges_to_number.append(edge_num)

    print_vertices(vertex_to_number)
    # print()
    print_edges(edges_to_number)

#function to generate graph
def generate_graph(street_map):
    global vertexes, edges, lines_to_vertex_map
    vertexes = []
    edges = []
    lines_to_vertex_map = {}
    generate_vertices(street_map)
    edges = calculate_edges_by_sorting(lines_to_vertex_map)
    numbering(vertexes, edges)

street_map = {}
CMDS = ("add", "rm", "mod", "gg")

valid_comands = {"add", "mod", "rm", "gg"}
street_name_regexp = r""
vertices_regexp = r""
import re

def parse_input(line):
    line = line.strip()
    if line.startswith("add") or line.startswith("mod"):
        assert line[:3] == "add" or line[:3] == "mod"
        
        assert re.match(r"^\s+$", line[3: line.find('"')]) is not None
        assert re.match(r"^\s+$", line[line.rfind('"') + 1: line.find('(')]) is not None
        
        # Verify street name
        street_name = line[line.find('"') + 1: line.rfind('"')]
        REGEX_STREET_NAME = r"^[a-zA-Z ]*[a-zA-Z][a-zA-Z ]*$"
        
        out = re.match(REGEX_STREET_NAME, street_name)
        if out is None:
            raise ParseExceptionError("Invalid Street Name")
        
        # Verify strret points
        REGEX_POINTS = r"^(\s*\(\s*-?[0-9]+\s*,\s*-?[0-9]+\s*\)\s*){2,}$"
        points_in_raw = line[line.find("("):]
        out = re.match(REGEX_POINTS, points_in_raw)
        if out is None:
            raise ParseExceptionError("Invalid Command")

        cmd = line[:3]
        street_name = line[line.find('"'): line.rfind('"') + 1]
        points = line[line.find("("):]
        points = "".join(points.split())
        points = points.replace(")", ") ")
        points = points[:len(points)-1]
        return cmd + " " + street_name + " " + points
        
    elif line.startswith(("rm")):
        assert line[:2] == "rm"
        assert re.match(r"^\s+$", line[2: line.find('"')]) is not None
        
        street_name = line[line.find('"') + 1: line.rfind('"')]
        REGEX_STREET_NAME = r"^[a-zA-Z ]*[a-zA-Z][a-zA-Z ]*$"
        assert line.rfind('"') == len(line) - 1

        out = re.match(REGEX_STREET_NAME, street_name)
        if out is None:
            raise ParseExceptionError("Invalid Street Name")
        
        return "rm " + line[line.find('"'): line.rfind('"') + 1]
        
    elif line.startswith("gg"):
        REGEX = r"^\s*(gg)\s*$"
        out = re.match(REGEX, line)
        if out is None:
            raise ParseExceptionError("Invalid gg command")
        return "gg"
    else:
        raise ParseExceptionError("Invalid Command a1")
    

def process_input(line):
    if not line.startswith(CMDS):
        raise ParseExceptionError("Invalid command. Enter any of below commands\n 1.add 2.mod 3.rm 4.gg")
           
    if line == "gg":
        generate_graph(street_map)
    else:
        
        (cmd, street_name, points_str) = line.split('"')
        cmd = cmd.strip()

        if cmd == "add":
            coordinates = points_str.strip().split()    #changing coor to tuple 
            coordinates = [eval(point) for point in coordinates]    #changing tuple to points
            if street_name in street_map:
                raise ParseExceptionError("street is already exists, cannot add again")
            
            street_map[street_name] = coordinates
        
        elif cmd == "mod":
           
            if street_name not in street_map:
                raise ParseExceptionError("street doesnt exist, try with different streetname")

            del street_map[street_name]
            
            coordinates = points_str.strip().split()
            coordinates = [eval(point) for point in coordinates]
            street_map[street_name] = coordinates

        elif cmd.lower() == "rm":
            if street_name not in street_map:
                raise ParseExceptionError("Street name doesnt exit")

            del street_map[street_name]

def driver_function():

    while True:
        try:
            inp = sys.stdin.readline()
            if inp == "":
                break
            
            updated_inp = parse_input(inp.strip())
            process_input(updated_inp.strip())
        except ParseExceptionError as ex:
            sys.stderr.write("Error: {0}\n".format(ex))
        except Exception as e:
            sys.stderr.write("Error: Invalid input\n")
    
    sys.exit(0)

if __name__ == "__main__":
    driver_function()