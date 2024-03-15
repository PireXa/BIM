import sys
import os
from PIL import Image

def extract_faces(cubemap_path):
    # Load cubemap texture
    cubemap = Image.open(cubemap_path)

    # Determine face size
    width, height = cubemap.size
    face_size = width // 4  # Assuming the texture is arranged in a 4x3 layout

    # Define face regions
    face_regions = {
        'front': (face_size, face_size, face_size * 2, face_size * 2),
        'back': (face_size * 3, face_size, face_size * 4, face_size * 2),
        'top': (face_size, 0, face_size * 2, face_size),
        'bottom': (face_size, height - face_size, face_size * 2, height),
        'left': (0, face_size, face_size, face_size * 2),
        'right': (face_size * 2, face_size, face_size * 3, face_size * 2)
    }

    # Create folder with the name of the original texture image
    folder_name = os.path.splitext(os.path.basename(cubemap_path))[0]
    os.makedirs(folder_name, exist_ok=True)

    # Extract and save each face
    for face_name, region in face_regions.items():
        face = cubemap.crop(region)
        face.save(os.path.join(folder_name, f"{face_name}.jpg"))

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python script.py <path_to_cubemap_texture>")
        sys.exit(1)
    
    cubemap_path = sys.argv[1]
    extract_faces(cubemap_path)

