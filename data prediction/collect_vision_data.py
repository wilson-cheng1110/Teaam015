import cv2
import numpy as np

def capture_images(camera_index=0, num_images=100):
    cap = cv2.VideoCapture(camera_index)
    images = []

    for _ in range(num_images):
        ret, frame = cap.read()
        if ret:
            images.append(frame)
            cv2.imshow('Frame', frame)
            cv2.waitKey(100)  # Display each frame for 100ms

    cap.release()
    cv2.destroyAllWindows()
    return images

if __name__ == "__main__":
    images = capture_images()
    # Save images for further processing
    for i, img in enumerate(images):
        cv2.imwrite(f'images/waste_image_{i}.jpg', img)