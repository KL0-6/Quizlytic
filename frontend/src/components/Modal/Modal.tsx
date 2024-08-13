import React from "react";

interface ModalProps
{
	isOpen: boolean;
	children: React.ReactNode;
}

const Modal: React.FC<ModalProps> = ({ isOpen, children }) => 
{
	if (!isOpen) return null;

	return (
		<div className="fixed inset-0 bg-black bg-opacity-50 flex items-center justify-center z-50">
			<div className="bg-gray-800 text-white rounded-lg shadow-lg p-6 max-w-lg w-full relative">
				{children}
			</div>
		</div>
	);
};

export default Modal;
