import streamlit as st
import requests

API_URL = "http://127.0.0.1:8000/game"

st.title("🎮 Text Adventure RPG")

# --- Initialize session state ---
if "phase" not in st.session_state:
    st.session_state.phase = "start"
if "char_name" not in st.session_state:
    st.session_state.char_name = ""
if "last_response" not in st.session_state:
    st.session_state.last_response = None
if "game_over" not in st.session_state:
    st.session_state.game_over = False

# --- Phase: Start Game ---
if st.session_state.phase == "start":
    if st.button("▶ Start Game"):
        st.session_state.phase = "choose_name"
        st.rerun()

# --- Phase: Choose Character Name ---
elif st.session_state.phase == "choose_name":
    st.write("📜 Enter your character's name:")
    name_input = st.text_input("Name:", st.session_state.char_name)
    if st.button("Next") and name_input.strip():
        st.session_state.char_name = name_input.strip()
        st.session_state.phase = "choose_class"
        st.rerun()

# --- Phase: Choose Class ---
elif st.session_state.phase == "choose_class":
    st.write(f"✨ Welcome, {st.session_state.char_name}! Choose your class:")
    class_choice = st.radio("Class:", ["Fighter", "Rogue"])
    if st.button("Create Character"):
        cls = 1 if class_choice == "Fighter" else 2
        req = {
            "action": "newchar",
            "name": st.session_state.char_name,
            "class_": cls
        }
        r = requests.post(API_URL, json=req)
        try:
            st.session_state.last_response = r.json()
            st.session_state.phase = "playing"
            st.rerun()
        except Exception as e:
            st.error(f"Failed to parse JSON: {e}")
            st.stop()

# --- Phase: Playing the Game ---
elif st.session_state.phase == "playing" and st.session_state.last_response:
    resp = st.session_state.last_response

    # Check for game over / win condition
    if "choices" in resp and "End Game" in resp["choices"]:
        st.success("🏆 Congratulations! You won the game!")
        st.write(resp.get("description", ""))
        st.session_state.game_over = True
        st.stop()

    # Show description
    if "description" in resp:
        st.write(resp["description"])

    # Show player stats
    if "player" in resp:
        player = resp["player"]
        st.subheader("🧑 Character Stats")
        st.write(f"HP: {player['hp']}/{player['maxHp']}")
        st.write(f"STR: {player['str']}")
        st.write(f"DEX: {player['dex']}")
        st.write(f"Level: {player['level']}")

        # Inventory & Equipment
        st.subheader("👜 Inventory")
        items = player.get("items", [])
        if items:
            for item in items:
                st.write(f"- {item}")
        else:
            st.write("Your inventory is empty.")

        st.subheader("⚔ Equipped Weapon")
        equipped = player.get("equippedWeapon", "None")
        st.write(equipped)

    # Show choices as buttons
    if "choices" in resp:
        for i, choice_text in enumerate(resp["choices"]):
            if st.button(choice_text, key=f"choice_{i}"):
                req = {"action": "choice", "value": i}
                r = requests.post(API_URL, json=req)
                try:
                    st.session_state.last_response = r.json()
                    st.rerun()
                except Exception as e:
                    st.error(f"Failed to parse JSON: {e}")
                    st.stop()
