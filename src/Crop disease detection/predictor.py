import matplotlib.pyplot as plt
import numpy as np
import tensorflow as tf
import cv2
from tensorflow import keras

####### This program will predict the disease type for maize crop 
####### Input will be taken from video cam (Logitech Webcam connected to the raspberry pi)
####### The previously trained model is loaded
####### The image is pre-processed and the predictions are made
####### The predictions will be wriiten on the video output

# setting parameters (Will be indirectly required for class names)
batch_size = 32
img_height = 256
img_width = 256

# Recalling the training dataset with labels as the directory name(Will be required for class names)
train_ds = tf.keras.utils.image_dataset_from_directory(
  directory="D:/Agri-bot data/train",
  validation_split=0.2,
  subset="training",
  seed=123,
  image_size=(img_height, img_width),
  batch_size=batch_size)

# Class names for final prediction
class_names = train_ds.class_names

# Load the trained model
model = keras.models.load_model("model.h5")

# Capturing from webcam
cap = cv2.VideoCapture(0)

while True :

  success, img = cap.read()  # Taking a frame as input image to make prediction
  img= cv2.flip(img, 1)      # Flip image laterally 

  img = cv2.resize(img,(256,256))  # Resize image according to model input
  img_array = tf.keras.utils.img_to_array(img) # Convert image to array
  img_array = tf.expand_dims(img_array, 0) # Create a batch

  predictions = model.predict(img_array) # Make Predictions
  score = tf.nn.softmax(predictions[0]) 
  
  # Setting parameters for text
  org = (20, 50)
  font = cv2.FONT_HERSHEY_SIMPLEX
  fontScale = 0.5
  color = (256, 256, 256)
  thickness = 2

  # Print the predictions in the terminal 
  print(class_names[np.argmax(score)])
  # Put the prediction as text on the image 
  image = cv2.putText(img, class_names[np.argmax(score)], org,font,
                   fontScale, color, thickness, cv2.LINE_AA)
  # Show image
  cv2.imshow('Disease Identification',image) 
  # Exit with the key q
  if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release() # After the loop release the cap object
cv2.destroyAllWindows() # Destroy all windows 
