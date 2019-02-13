#pragma once
#include <atomic>
#include "PixisCamera.h"
#include "PictureControl.h"
#include "Control.h"
#include "CameraSettingsControl.h"
#include "PictureStats.h"
#include "PictureSaver.h"
#include "commonTypes.h"
#include "PlotCtrl.h"

class SingleCameraWindow : public CDialogEx
{
	public:
		SingleCameraWindow(CWnd* pParent = NULL);	
		BOOL OnInitDialog();
		void handleEnter();
		void OnPaint();
		HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
		HCURSOR OnQueryDragIcon();
		void OnSize( UINT nType, int cx, int cy );
		void handleArmPress();
		void handleClose();
		void OnTimer ( UINT_PTR id );
		void handleDisarmPress();
		void initializeControls();
		void passExposureMode();
		void passCameraMode();
		afx_msg LRESULT handleNewPics( WPARAM wParam, LPARAM lParam );
		void pictureRangeEditChange( UINT id );
		void OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* scrollbar );
		void handleSoftwareTrigger();
		void OnMouseMove(UINT flags, CPoint point);
		void OnRButtonUp( UINT stuff, CPoint clickLocation );
		void passSetLocationsButton();
		void DoDataExchange( CDataExchange* pDX );
		void handleSetTemperature ( );
		
	private:
		PictureControl picture;
		CameraSettingsControl settings;
		PixisCamera cameraController;
		PictureStats stats;
		PictureSaver saver;
		unsigned int currentRepNumber;
		std::string runExposureMode;
		std::vector<std::vector<long>> images;
		unsigned int imageWidth;
		brushMap mainBrushes;
		rgbMap mainRGBs;
		fontMap mainFonts;
		bool isRunning;
		std::atomic<bool> triggerThreadFlag;
		PlotCtrl *horGraph, *vertGraph;
		std::vector<Gdiplus::Pen*> plotPens, brightPlotPens;
		std::vector<Gdiplus::SolidBrush*> plotBrushes, brightPlotBrushes;
		CFont* plotfont;
	protected:
		//virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
		HICON m_hIcon;

	DECLARE_MESSAGE_MAP()
};
