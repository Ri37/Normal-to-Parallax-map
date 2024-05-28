import numpy as np
from PIL import Image
import matplotlib.pyplot as plt
from to_height_advanced import convert_normal_to_height_advanced
from to_height import convert_normal_to_height_with_individual_maps
from to_normal import convert_height_to_normal

# scale up the differences to see errors
scale = 1.0

#original_height_map = Image.open("height.png").convert("L")
original_height_map = Image.open("stone_wall_height.png").convert("L")
#original_height_map = Image.open("round_height.png").convert("L")

# Generate normal map
normal_map = convert_height_to_normal(original_height_map)
# Generate height maps
height_maps_list, final_height_map = convert_normal_to_height_with_individual_maps(normal_map)
# Generate advanced height map
advanced_height_map = convert_normal_to_height_advanced(normal_map)

# Calculate differences
diff_simple = np.abs(np.array(original_height_map, dtype=np.float32) - np.array(final_height_map, dtype=np.float32)) * scale
diff_advanced = np.abs(np.array(original_height_map, dtype=np.float32) - np.array(advanced_height_map, dtype=np.float32)) * scale

fig, axes = plt.subplots(3, 4, figsize=(20, 15))

# First row: original height map, generated normal map, first and second height maps from the list
axes[0, 0].imshow(original_height_map, cmap='gray')
axes[0, 0].set_title("Original Height Map")
axes[0, 0].axis('off')

axes[0, 1].imshow(normal_map)
axes[0, 1].set_title("Generated Normal Map")
axes[0, 1].axis('off')

axes[0, 2].imshow(height_maps_list[0], cmap='gray')
axes[0, 2].set_title("Generated Height Map T->B")
axes[0, 2].axis('off')

axes[0, 3].imshow(height_maps_list[1], cmap='gray')
axes[0, 3].set_title("Generated Height Map B->T")
axes[0, 3].axis('off')

# Second row: final simple height map, advanced height map, third and fourth height maps from the list
axes[1, 0].imshow(final_height_map, cmap='gray')
axes[1, 0].set_title("Final Simple Height Map")
axes[1, 0].axis('off')

axes[1, 1].imshow(advanced_height_map, cmap='gray')
axes[1, 1].set_title("Advanced Generated Height Map")
axes[1, 1].axis('off')

axes[1, 2].imshow(height_maps_list[2], cmap='gray')
axes[1, 2].set_title("Generated Height Map L->R")
axes[1, 2].axis('off')

axes[1, 3].imshow(height_maps_list[3], cmap='gray')
axes[1, 3].set_title("Generated Height Map R->L")
axes[1, 3].axis('off')

# Third row: difference images
axes[2, 0].imshow(diff_simple, cmap='gray', vmin=0, vmax=255)
axes[2, 0].set_title("Original - Simple Height Map")
axes[2, 0].axis('off')

axes[2, 1].imshow(diff_advanced, cmap='gray', vmin=0, vmax=255)
axes[2, 1].set_title("Original - Advanced Height Map")
axes[2, 1].axis('off')

# Hide the remaining empty subplots
axes[2, 2].axis('off')
axes[2, 3].axis('off')

plt.tight_layout()
plt.show()
