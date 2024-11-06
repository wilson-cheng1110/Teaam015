import cv2
import os

def preprocess_images(image_folder='images', output_folder='processed_images'):
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)

    for filename in os.listdir(image_folder):
        img = cv2.imread(os.path.join(image_folder, filename))
        img_resized = cv2.resize(img, (224, 224))  # Resize to 224x224 for model input
        cv2.imwrite(os.path.join(output_folder, filename), img_resized)

if __name__ == "__main__":
    preprocess_images()