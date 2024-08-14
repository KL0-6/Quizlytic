import React, { useEffect } from "react";

interface NotificationProps {
	message: string;
	type: "success" | "error";
	onClose: () => void;
}

const Notification: React.FC<NotificationProps> = ({ message, type, onClose }) => {
	useEffect(() => 
    {
		const timer = setTimeout(() => 
        {
			onClose();
		}, 5000);

		return () => clearTimeout(timer);
	}, [onClose]);

	return (
		<div
			className={`fixed inset-x-4 top-4 p-4 rounded-md shadow-md z-50 md:max-w-sm md:mx-auto transition-transform transform ${
				type === "success" ? "bg-green-600 text-white" : "bg-red-600 text-white"
			}`}
			style={{ maxWidth: '90%', marginLeft: 'auto', marginRight: 'auto' }}
		>
			<div className="flex items-center justify-between">
				<span className="flex-1">{message}</span>
				<button
					onClick={onClose}
					className="ml-4 text-white focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-white text-xl leading-none"
				>
					&times;
				</button>
			</div>
		</div>
	);
};

export default Notification;
