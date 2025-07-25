import asyncio
from telegram import Bot
from telegram.constants import ParseMode
from dotenv import load_dotenv
import os

load_dotenv()


BOT_TOKEN = os.getenv('BOT_TOKEN')
CHAT_ID = os.getenv('CHAT_ID')

async def send_telegram_alert(message):
    bot = Bot(token=BOT_TOKEN)
    await bot.send_message(chat_id=CHAT_ID, text=message, parse_mode=ParseMode.HTML)

if __name__ == '__main__':
    message = "⚠️ <b>Cảnh báo!</b> Nhiệt độ vượt ngưỡng cho phép!"
    asyncio.run(send_telegram_alert(message))
