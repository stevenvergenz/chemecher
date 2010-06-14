-- MySQL dump 10.13  Distrib 5.1.41, for debian-linux-gnu (x86_64)
--
-- Host: localhost    Database: CheMecher
-- ------------------------------------------------------
-- Server version	5.1.41-3ubuntu12.3

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `compounds`
--

DROP TABLE IF EXISTS `compounds`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `compounds` (
  `MechID` int(11) NOT NULL,
  `CpdID` int(11) NOT NULL,
  `ShortName` char(2) NOT NULL,
  `LongName` char(30) NOT NULL,
  `State` enum('homo','hetero','aq','s','l','g') NOT NULL,
  `Transition` enum('linear','atan') DEFAULT NULL,
  `InitConc` double NOT NULL,
  PRIMARY KEY (`MechID`,`CpdID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `compounds`
--

LOCK TABLES `compounds` WRITE;
/*!40000 ALTER TABLE `compounds` DISABLE KEYS */;
/*!40000 ALTER TABLE `compounds` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mechs`
--

DROP TABLE IF EXISTS `mechs`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `mechs` (
  `MechID` int(11) NOT NULL,
  `MechName` char(50) NOT NULL,
  `ReportStep` double NOT NULL,
  `TimeStep` double NOT NULL,
  `InitTime` int(11) NOT NULL,
  `MaxTime` int(11) NOT NULL,
  `DebugStart` int(11) NOT NULL,
  `DebugEnd` int(11) NOT NULL,
  `Precis` double NOT NULL,
  PRIMARY KEY (`MechID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mechs`
--

LOCK TABLES `mechs` WRITE;
/*!40000 ALTER TABLE `mechs` DISABLE KEYS */;
/*!40000 ALTER TABLE `mechs` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `steps`
--

DROP TABLE IF EXISTS `steps`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `steps` (
  `MechID` int(11) NOT NULL,
  `StepID` int(11) NOT NULL,
  `StepName` char(30) NOT NULL,
  `Kplus` double NOT NULL,
  `Kminus` double NOT NULL,
  `Reac1ID` int(11) NOT NULL,
  `Reac2ID` int(11) DEFAULT NULL,
  `Reac3ID` int(11) DEFAULT NULL,
  `Prod1ID` int(11) NOT NULL,
  `Prod2ID` int(11) DEFAULT NULL,
  `Prod3ID` int(11) DEFAULT NULL,
  PRIMARY KEY (`MechID`,`StepID`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `steps`
--

LOCK TABLES `steps` WRITE;
/*!40000 ALTER TABLE `steps` DISABLE KEYS */;
/*!40000 ALTER TABLE `steps` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2010-06-13 14:02:22
