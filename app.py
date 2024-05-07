from flask import Flask, jsonify, request  # type: ignore
from flask_cors import CORS  # type: ignore
import firebase_admin  # type: ignore
from firebase_admin import credentials, firestore  # type: ignore

# Initialize Flask app
app = Flask(__name__)
CORS(app)

# Initialize Firebase Firestore
cred = credentials.Certificate("serviceAccountKey.json")
firebase_admin.initialize_app(cred)
db = firestore.client()


# Routes
@app.route("/contacts", methods=["GET"])
def get_contacts():
    contacts = []
    # Retrieve contacts from Firestore
    docs = db.collection("contacts").get()
    for doc in docs:
        contact = doc.to_dict()
        contact["id"] = doc.id
        contacts.append(contact)
    return jsonify(contacts)


@app.route("/contacts", methods=["POST"])
def add_contact():
    data = request.json
    # Add contact to Firestore
    db.collection("contacts").add(data)
    return jsonify({"message": "Contact added successfully"})


@app.route("/contacts/<contact_id>", methods=["PUT"])
def update_contact(contact_id):
    data = request.json
    # Update contact in Firestore
    db.collection("contacts").document(contact_id).update(data)
    return jsonify({"message": "Contact updated successfully"})


@app.route("/contacts/<contact_id>", methods=["DELETE"])
def delete_contact(contact_id):
    # Delete contact from Firestore
    db.collection("contacts").document(contact_id).delete()
    return jsonify({"message": "Contact deleted successfully"})


if __name__ == "__main__":
    app.run(debug=True)
