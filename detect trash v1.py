import cv2
import requests
import json
from inference_sdk import InferenceHTTPClient

# Initialize the Inference client
CLIENT = InferenceHTTPClient(
    api_url="https://detect.roboflow.com",
    api_key="API_KEY"
)

# Function to run inference on a frame
def run_inference(frame):
    # Encode the frame as JPEG
    _, buffer = cv2.imencode('.jpg', frame)
    frame_data = buffer.tobytes()

    # Call the inference API
    result = CLIENT.infer(frame_data, model_id="w-wi4ow/1")
    return result

# Start capturing video from the webcam
cap = cv2.VideoCapture(0)

if not cap.isOpened():
    print("Error: Could not open webcam.")
    exit()

while True:
    # Capture frame-by-frame
    ret, frame = cap.read()
    if not ret:
        print("Error: Could not read frame.")
        break

    # Run inference on the captured frame
    inference_result = run_inference(frame)

    # Process the inference result (for example, draw bounding boxes)
    # Here you would typically parse the result and draw it on the frame
    # For demonstration, we will just print the result
    print(json.dumps(inference_result, indent=2))

    # Display the resulting frame
    cv2.imshow('Webcam Feed', frame)

    # Break the loop on 'q' key press
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release the webcam and close windows
cap.release()
cv2.destroyAllWindows()