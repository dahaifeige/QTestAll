#pragma once
#ifndef BITARRAY_H
#define BITARRAY_H
#include <QBitArray>

enum Status
{
	Status_Connect,
	Status_Disconnect,
	Status_Save,
	Status_Single,
	Status_Continuous,
	Status_Stop,
	Status_Pause,
	Status_Record,
	Status_ZoomIn,
	Status_ZoomOut,
	Status_ZoomTo100,
	Status_ZoomToFit,
	Status_DisSelected
};
class BitArray :public QBitArray
{
public:
	BitArray() {
		resize(16);
		fill(true);
		SetItemStatus(Status_Connect, false);
	}
	bool GetItemStatus(int status) { return at(status); }
	void SetItemStatus(int status, bool ok) { setBit(status, ok); }
	//true gray disable

	void SetConnectStatus() {
		SetItemStatus(Status_Connect, true);
		SetItemStatus(Status_Disconnect, false);

		SetItemStatus(Status_Save, true);

		SetItemStatus(Status_Single, false);
		SetItemStatus(Status_Continuous, false);

		SetItemStatus(Status_Stop, true);
		SetItemStatus(Status_Pause, true);
		SetItemStatus(Status_Record, false);

		SetItemStatus(Status_ZoomIn, true);
		SetItemStatus(Status_ZoomOut, true);
		SetItemStatus(Status_ZoomTo100, true);
		SetItemStatus(Status_ZoomToFit, true);
	};
	void SetDisConnectStatus() { 
		SetItemStatus(Status_Connect, false);
		SetItemStatus(Status_Disconnect, true);

		SetItemStatus(Status_Save, true);

		SetItemStatus(Status_Single, true);
		SetItemStatus(Status_Continuous, true);

		SetItemStatus(Status_Stop, true);
		SetItemStatus(Status_Pause, true);
		SetItemStatus(Status_Record, true);

		SetItemStatus(Status_ZoomIn, true);
		SetItemStatus(Status_ZoomOut, true);
		SetItemStatus(Status_ZoomTo100, true);
		SetItemStatus(Status_ZoomToFit, true);

		SetItemStatus(Status_Connect, true);
		SetItemStatus(Status_Disconnect, true);
	}
	void SetSaveStatus() {
		SetItemStatus(Status_Save, true);
	}
	void SetSingleStatus() {
		SetItemStatus(Status_Connect, true);
		SetItemStatus(Status_Disconnect, false);

		SetItemStatus(Status_Save, false);

		SetItemStatus(Status_Single, true);
		SetItemStatus(Status_Continuous, false);

		SetItemStatus(Status_Stop, true);
		SetItemStatus(Status_Pause, true);
		SetItemStatus(Status_Record, false);

		SetItemStatus(Status_ZoomIn, false);
		SetItemStatus(Status_ZoomOut, false);
		SetItemStatus(Status_ZoomTo100, false);
		SetItemStatus(Status_ZoomToFit, false);
	}
	void SetContinuousStatus() {
		SetItemStatus(Status_Connect, true);
		SetItemStatus(Status_Disconnect, false);

		SetItemStatus(Status_Save, false);

		SetItemStatus(Status_Single, false);
		SetItemStatus(Status_Continuous, true);

		SetItemStatus(Status_Stop, false);
		SetItemStatus(Status_Pause, true);
		SetItemStatus(Status_Record, false);

		SetItemStatus(Status_ZoomIn, false);
		SetItemStatus(Status_ZoomOut, false);
		SetItemStatus(Status_ZoomTo100, false);
		SetItemStatus(Status_ZoomToFit, false);
	}
	void SetStopStatus() {
		SetItemStatus(Status_Connect, true);
		SetItemStatus(Status_Disconnect, false);

		SetItemStatus(Status_Save, false);

		SetItemStatus(Status_Single, false);
		SetItemStatus(Status_Continuous, false);

		SetItemStatus(Status_Stop, true);
		SetItemStatus(Status_Pause, true);
		SetItemStatus(Status_Record, false);

		SetItemStatus(Status_ZoomIn, false);
		SetItemStatus(Status_ZoomOut, false);
		SetItemStatus(Status_ZoomTo100, false);
		SetItemStatus(Status_ZoomToFit, false);
	}
	void SetRecordStatus() {
		SetItemStatus(Status_Connect, true);
		SetItemStatus(Status_Disconnect, false);

		SetItemStatus(Status_Save, false);

		SetItemStatus(Status_Single, false);
		SetItemStatus(Status_Continuous, true);

		SetItemStatus(Status_Stop, false);
		SetItemStatus(Status_Pause, false);
		SetItemStatus(Status_Record, true);

		SetItemStatus(Status_ZoomIn, false);
		SetItemStatus(Status_ZoomOut, false);
		SetItemStatus(Status_ZoomTo100, false);
		SetItemStatus(Status_ZoomToFit, false);
	}
	void SetPauseStatus() {
		SetItemStatus(Status_Connect, true);
		SetItemStatus(Status_Disconnect, false);

		SetItemStatus(Status_Save, false);

		SetItemStatus(Status_Single, false);
		SetItemStatus(Status_Continuous, true);

		SetItemStatus(Status_Stop, false);
		SetItemStatus(Status_Pause, !GetItemStatus(Status_Pause));
		SetItemStatus(Status_Record, GetItemStatus(Status_Record));

		SetItemStatus(Status_ZoomIn, false);
		SetItemStatus(Status_ZoomOut, false);
		SetItemStatus(Status_ZoomTo100, false);
		SetItemStatus(Status_ZoomToFit, false);
	}
	void SetZoomIn() {
		SetItemStatus(Status_ZoomIn, true);
		SetItemStatus(Status_ZoomOut, false);
		SetItemStatus(Status_ZoomTo100, false);
		SetItemStatus(Status_ZoomToFit, false);
	}
	void SetZoomOut() {
		SetItemStatus(Status_ZoomIn, false);
		SetItemStatus(Status_ZoomOut, true);
		SetItemStatus(Status_ZoomTo100, false);
		SetItemStatus(Status_ZoomToFit, false);
	}
	void SetZoomTo100() {
		SetItemStatus(Status_ZoomIn, false);
		SetItemStatus(Status_ZoomOut, false);
		SetItemStatus(Status_ZoomTo100, true);
		SetItemStatus(Status_ZoomToFit, false);
	}
	void SetZoomToFit() {
		SetItemStatus(Status_ZoomIn, false);
		SetItemStatus(Status_ZoomOut, false);
		SetItemStatus(Status_ZoomTo100, false);
		SetItemStatus(Status_ZoomToFit, true);
	}
};
#endif