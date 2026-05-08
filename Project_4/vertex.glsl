#version 430

void main(void) {
    switch (gl_VertexID) {
        case 0:
            gl_Position = vec4(0.25, -0.25, 0.0, 1.0);
            break;
        case 1:
            gl_Position = vec4(-0.25, -0.25, 0.0, 1.0);
            break;
        case 2:
            gl_Position = vec4(0.25, 0.25, 0.0, 1.0);
            break;
    }
}
