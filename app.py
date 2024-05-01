from flask import Flask, render_template, request, jsonify  # type: ignore
import firebase_admin  # type: ignore
from firebase_admin import credentials, firestore  # type: ignore

app = Flask(__name__)

# Initialize Firestore with the service account credentials
cred = credentials.Certificate("serviceAccountKey.json")
firebase_admin.initialize_app(cred)
db = firestore.client()


# Route to render the customer management page
@app.route("/customers")
def customers():
    return render_template("customers.html")


# Route to handle adding/editing a customer
@app.route("/add_edit_customer", methods=["POST"])
def add_edit_customer():
    customer_data = request.form
    customer_id = customer_data.get("customer_id")
    name = customer_data.get("name")
    email = customer_data.get("email")
    phone = customer_data.get("phone")

    if customer_id:  # If customer_id is present, update the existing customer
        db.collection("customers").document(customer_id).set(
            {"name": name, "email": email, "phone": phone}
        )
    else:  # If customer_id is not present, add a new customer
        db.collection("customers").add({"name": name, "email": email, "phone": phone})

    return jsonify({"success": True})


# Route to fetch all customers from Firestore
@app.route("/get_customers", methods=["GET"])
def get_customers():
    customers_ref = db.collection("customers")
    customers = [doc.to_dict() for doc in customers_ref.stream()]
    return jsonify(customers)


# Route to fetch interactions from Firestore
@app.route("/interactions")
def view_interactions():
    interactions_ref = db.collection("interactions")
    interactions = [doc.to_dict() for doc in interactions_ref.stream()]
    return render_template("interactions.html", interactions=interactions)


# Route to fetch products from Firestore
@app.route("/products")
def view_products():
    products_ref = db.collection("products")
    products = [doc.to_dict() for doc in products_ref.stream()]
    return render_template("products.html", products=products)


# Route to fetch orders from Firestore
@app.route("/orders")
def view_orders():
    orders_ref = db.collection("orders")
    orders = [doc.to_dict() for doc in orders_ref.stream()]
    return render_template("orders.html", orders=orders)


if __name__ == "__main__":
    app.run(debug=True)
