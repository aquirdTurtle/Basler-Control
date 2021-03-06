﻿#pragma once
#include "boost/container/vector.hpp"
#include "windows.h"

// a wrapper around a 1D matrix that allows for 2D (row/collum) access styles. Mostly used to represetn images.
template <class type>
class Matrix
{
	public:
		Matrix();
		Matrix( UINT rowsInGrid, UINT colsInGrid );
		Matrix( UINT rowsInGrid, UINT colsInGrid, type initValue );
		Matrix( UINT rowsInGrid, UINT colsInGrid, std::vector<type> init1D );
		
		type operator()( UINT row, UINT col ) const;
		type & operator()( UINT row, UINT col );
		type operator()( POINT p ) const;
		type & operator()( POINT p );
		UINT getRows( );
		UINT getCols( );
		Matrix<type> submatrix( UINT rowOffset, UINT rowSubSpan, UINT colOffset, UINT colSubSpan );
		std::string print( );
		void updateString( );
		UINT size( ) const;
		// typename tells the compiler that std::vector<type>::iterator will be a type.
		typename boost::container::vector<type>::iterator begin( ) { return data.begin( ); }
		typename boost::container::vector<type>::const_iterator begin( ) const { return data.begin( ); }
		typename boost::container::vector<type>::iterator end( ) { return data.end( ); }
		typename boost::container::vector<type>::const_iterator end( ) const { return data.end( ); }
		// need to use the boost version because the std version doesn't do std::vector<bool> properly.
		boost::container::vector<type> data;
	private:
		UINT rows, cols;
		// the following string is only updated if in debug mode.
		std::string currMatrix;
};

template<class type>
Matrix<type>::Matrix( ) :
	rows( 0 ),
	cols( 0 )
{
}


// the array gets sized only once in the constructor.
template<class type>
Matrix<type>::Matrix( UINT rowsInGrid, UINT colsInGrid ) :
	rows( rowsInGrid ),
	cols( colsInGrid ),
	data( rowsInGrid*colsInGrid )
{}


template<class type>
Matrix<type>::Matrix( UINT rowsInGrid, UINT colsInGrid, type initValue ) :
	rows( rowsInGrid ),
	cols( colsInGrid ),
	data( rowsInGrid*colsInGrid, initValue )
{}


template<class type>
Matrix<type>::Matrix( UINT rowsInGrid, UINT colsInGrid, std::vector<type> init1D ) :
	rows( rowsInGrid ),
	cols( colsInGrid ),
	data( init1D.begin( ), init1D.end( ) )
{
	if ( data.size( ) != rows * cols )
	{
		thrower( "ERROR: Initialized matrix with 1d vector whose size did not match the initializing row and column #."
				 "Lengths were: " + str( data.size( ) ) + ", " + str( rows ) + ", and " + str( cols )
				 + " respectively." );
	}
}


template <class type>
void Matrix<type>::updateString( )
{
#ifdef _DEBUG
	currMatrix = print( );
#endif
	return;
}


template <class type>
UINT Matrix<type>::size( ) const
{
	return data.size( );
}


template <class type>
Matrix<type> Matrix<type>::submatrix( UINT rowOffset, UINT rowSubSpan, UINT colOffset, UINT colSubSpan )
{
	if ( rowOffset + rowSubSpan > rows || colOffset + colSubSpan > cols )
	{
		thrower( "ERROR: submatrix extends beyond matrix bounds!" );
	}
	Matrix<type> subM( 0, 0 );
	// might be faster to use insert.
	for ( auto rowInc : range( rowSubSpan ) )
	{
		subM.data.insert( subM.data.end( ), data.begin( ) + (rowOffset + rowInc) * cols + colOffset,
						  data.begin( ) + (rowOffset + rowInc) * cols + colOffset + colSubSpan );
	}
	subM.rows = rowSubSpan;
	subM.cols = colSubSpan;
	updateString( );
	return subM;
}


template <class type>
std::string Matrix<type>::print( )
{
	std::string printStr;
	UINT counter = 0;
	for ( auto elem : *this )
	{
		printStr += str( elem ) + ", ";
		if ( ++counter % cols == 0 )
		{
			printStr += ";\n";
		}
	}
	return printStr;
}


/*
	Equivalent to passing row and column explicitly with p.x as the column and p.y as the row. Careful, x counts from 
	left to right but y counts from top to bottom.
*/
template<class type>
type Matrix<type>::operator()( POINT p ) const
{
	if ( p.y >= rows )
	{
		thrower( "ERROR: row index out of range during Matrix access! row was " + str( p.y ) );
	}
	if ( p.x >= cols )
	{
		thrower( "ERROR: col index out of range during Matrix access! col was " + str( p.x ) );
	}

	UINT rowOffset( p.y * cols );
	UINT index = rowOffset + p.x;
	return data[index];
}

template<class type>
type & Matrix<type>::operator()( POINT p )
{
	if ( p.y >= rows )
	{
		thrower( "ERROR: row index out of range during Matrix access! row was " + str( p.y ) );
	}
	if ( p.x >= cols )
	{
		thrower( "ERROR: col index out of range during Matrix access! col was " + str( p.x ) );
	}
	UINT rowOffset( p.y * cols );
	UINT index = rowOffset + p.x;
	updateString( );
	return data[index];
}

template<class type>
type Matrix<type>::operator()( UINT row, UINT col ) const
{
	if ( row >= rows )
	{
		thrower( "ERROR: row index out of range during Matrix access! row was " + str( row ) );
	}
	if ( col >= cols )
	{
		thrower( "ERROR: col index out of range during Matrix access! col was " + str( col ) );
	}
	UINT rowOffset( row * cols );
	UINT index = rowOffset + col;
	return data[index];
}

template<class type>
type & Matrix<type>::operator()( UINT row, UINT col )
{
	if ( row >= rows )
	{
		thrower( "ERROR: row index out of range during Matrix access! row was " + str( row) );
	}
	if ( col >= cols )
	{
		thrower( "ERROR: col index out of range during Matrix access! col was " + str(col) );
	}
	UINT rowOffset( row * cols );
	UINT index = rowOffset + col;
	updateString( );
	return data[index];
}


template <class type>
UINT Matrix<type>::getCols( )
{
	updateString( );
	return cols;
}


template <class type>
UINT Matrix<type>::getRows( )
{
	updateString( );
	return rows;
}

