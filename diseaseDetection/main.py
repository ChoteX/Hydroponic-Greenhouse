# Import required libraries
import torch
import torch.nn as nn
import torchvision.models as models
from torchvision import transforms
from PIL import Image
import warnings

# Suppress specific user warnings related to deprecation
warnings.filterwarnings("ignore", category=UserWarning, message=".*deprecated.*")

# Load EfficientNet-B0 model without pretrained weights and with 2 output classes
model = models.efficientnet_b0(pretrained=False, num_classes=2)

# Modify the first convolution layer to accept 1-channel (grayscale) images
model.features[0][0] = nn.Conv2d(
    in_channels=1,  # Change input channels from 3 (RGB) to 1 (grayscale)
    out_channels=model.features[0][0].out_channels,
    kernel_size=model.features[0][0].kernel_size,
    stride=model.features[0][0].stride,
    padding=model.features[0][0].padding,
    bias=False
)

# Replace the classifier layer to match the number of target classes
num_classes = 2
in_features = model.classifier[1].in_features
model.classifier[1] = nn.Linear(in_features, num_classes)

# Load the trained model weights from a .pth file
model.load_state_dict(torch.load(r"D:\\Lettuce Classifier\\lettuce_classifier.pth", map_location=torch.device('cpu')))

# Set the model to evaluation mode
model.eval()

# Define class labels
classes = ["Healthy", "Diseased"]

# Define the image preprocessing steps
transform = transforms.Compose([
    transforms.Resize((224, 224)),     # Resize image to 224x224
    transforms.ToTensor(),             # Convert image to tensor
    transforms.Normalize(mean=[0.5], std=[0.5])  # Normalize grayscale image
])

# Load the image, convert to grayscale (L mode), and apply transformations
image_path = r"C:\Users\Home\Desktop\test.jpeg"
image = Image.open(image_path).convert('L')
input_tensor = transform(image).unsqueeze(0)  # Add batch dimension

# Move model and input tensor to GPU if available, else use CPU
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
input_tensor = input_tensor.to(device)
model = model.to(device)

# Perform inference without tracking gradients
with torch.no_grad():
    output = model(input_tensor)  # Get model output
    predicted_class = torch.argmax(output, dim=1).item()  # Get predicted class index

# Print the predicted class label
print(f"Prediction: {classes[predicted_class]}")
